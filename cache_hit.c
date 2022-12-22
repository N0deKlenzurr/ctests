#define CACHE_SIZE 1024
#define BLOCK_SIZE 64
#define NUM_SETS 16

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


void init_cache(cache_t *cache) {

	for (int i = 0; i < NUM_SETS; i++) {
		cache_set_t* set = &cache->sets[i];

		for (int j = 0; j < NUM_LINES_PER_SET; j++) {
			cache_line_t* line = &set->lines[j];
			line->valid = 0;
			line->dirty = 0;
		}
		set->lru = 0;
	}
}


void access_memory(cache_t* cache, int addr)
{
	int set_index = cache->map(addr);
	cache_set_t* set = &cache->sets[set_index];

	int hit = 0;
	for (int i = 0; i < NUM_LINES_PER_SET; i++) {
		cache_line_t* line = &set->lines[i];

		if (line->valid && line->tag == addr >> 6) {
			set->lru = (i + NUM_LINES_PER_SET - 1) % NUM_LINES_PER_SET;
			hit = 1;
			break;
		}
	}

	if (!hit) {
		cache_line_t* line = &set->lines[set->lru];
		line->valid = 1;
		line->tag = addr >> 6;
		set->lru = (set->lru + NUM_LINES_PER_SET - 1) % NUM_LINES_PER_SET;
	}
}

int main()
{
	cache_t cache;
	init_cache(&cache);

	cache.map = [](int addr) { return addr % NUM_SETS}
}
