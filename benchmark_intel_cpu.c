#include <immintrin.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <emmintrin.h>
#include <inttypes.h>


static inline uint64_t rdtscp(void)
{
	uint64_t rax, rdx;
	__asm__ __volatile__(
		"rdtscp"
		: "+a" (rax), "=d" (rdx)
		:: "%rcx", "memory"
	);

	// combine 32-bit rdx and rax into a single 64-bit value
	return (((uint64_t)rdx << 32) | rax);
}

int main(int argc, char* argv[])
{
	/* 
		use mmap() to create a new virtual mapping,
		then zero the structure out.
	*/

	unsigned char* buffer;

	buffer = mmap(NULL, 256 * 4096, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

	memset(buffer, 0, 256 * 4096);

	// flush every page from every cache
	for( size_t k = 0; k < 256; ++k) {
		_mm_clflush(buffer + k * 4096);
	}
	_mm_mfence();

	// initializing this page will add it to cache
	buffer[42 * 4096] = 0xff;

	size_t lowest_k = 0;
	uint64_t lowest_dt = 0xffffffff;
	for (size_t k = 0; k < 256; k++) {
		uint64_t t0, dt;

		t0 = rdtscp();
		*(volatile char *)(buffer + 4096 * k);
		dt = rdtscp() - t0;

		if (dt < lowest_dt) {
			lowest_dt = dt;
			lowest_k = k;
		}
		printf("accessing page %02d in %" PRIu64 " cycles.\n", k, dt);
	}

	printf("[!] Fastest cache page is %d\n", lowest_k);
	return 0;
}