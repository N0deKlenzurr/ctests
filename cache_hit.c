/*

debug this program
------------------

gdb <program name>
break access_memory
run
next
print $eax

*/

#include <stdio.h>
#include <stdlib.h>

#define CACHE_SIZE 1024
#define BLOCK_SIZE 64
#define NUM_SETS 16
#define NUM_LINES_PER_SET 10

typedef struct {
	int tag;
	int valid;
	int dirty;
	unsigned char data[BLOCK_SIZE];
} cache_line_t;

typedef struct {
	cache_line_t lines[NUM_LINES_PER_SET];
	int lru;
} cache_set_t;

typedef struct {
	cache_set_t sets[NUM_SETS];
	int (*map)(int addr);
} cache_t;

int map_to_sets(int addr) {
	return addr % NUM_SETS;
}

void init_cache(cache_t *cache) {
	printf("[*] Initializing Cache Memory!\n");
	for (int i = 0; i < NUM_SETS; i++) {
		cache_set_t* set = &cache->sets[i];

		for (int j = 0; j < NUM_LINES_PER_SET; j++) {
			cache_line_t* line = &set->lines[j];
			line->valid = 0;
			line->dirty = 0;
		}
		set->lru = 0;
	}

	cache->map = map_to_sets;
}


void access_memory(cache_t* cache, int addr)
{
	int set_index = cache->map(addr);
	cache_set_t* set = &cache->sets[set_index];

	int hit = 0;

	printf("[+] Accessing Cache Memory!n\n");
	for (int i = 0; i < NUM_LINES_PER_SET; i++) {
		cache_line_t* line = &set->lines[i];

		printf("[!] Checking Cache line %d\n", i);
		if (line->valid && line->tag == addr >> 6) {
			set->lru = (i + NUM_LINES_PER_SET - 1) % NUM_LINES_PER_SET;
			hit = 1;
			break;
		}
	}

	if (!hit) {
		printf("[!] Cache Miss Recorded!\n");
		cache_line_t* line = &set->lines[set->lru];
		line->valid = 1;
		line->tag = addr >> 6;
		set->lru = (set->lru + NUM_LINES_PER_SET - 1) % NUM_LINES_PER_SET;
	}
}


int main(int argc, char* argv[])
{
	cache_t cache;
	init_cache(&cache);
	int addr = 896456;

	map_to_sets(addr);

	for (int i = 0; i < 1000; i++) {
		access_memory(&cache, 0);
	}

	return 0;
}

