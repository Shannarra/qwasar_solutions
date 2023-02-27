#ifndef MY_MALLOC_HDR
#define MY_MALLOC_HDR
#define size_t unsigned int

typedef enum free_type {
  NOT_FREE, FREE
} free_type;

typedef struct chunk {
  size_t _chunk_sz;

  struct chunk *next, /* sizeof(chunk*) = 8 */
                *prev,
                *next_free,
                *prev_free;
  free_type free;

} chunk;

typedef struct mem_session {
  chunk *_first_chunk, /* sizeof(chunk*) = 8 */
         *_last_chunk,
         *_first_free_chunk,
         *_last_free_chunk;

  size_t _chunks_allocated;
} mem_session;


void *my_malloc(size_t bytes);
void *my_calloc(size_t nmemb, size_t size);
void *my_realloc(void *ptr, size_t size);
void my_free(void *ptr);

int   create_session();
chunk *find_free_chunk( size_t bytes );

void merge_chunks(chunk *a, chunk *b);

#endif
