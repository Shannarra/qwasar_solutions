
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "my_malloc.h"

static mem_session *_session = NULL;

/*
    If you want to see debug informations about allocations
    and/or memory addresses just uncomment the line bellow:
*/
//#define MY_MALLOC_DEBUG

#ifdef MY_MALLOC_DEBUG
  #include <stdarg.h>
  static int _debug_itr = 0;
#endif

static void debug(char* fmt, ...) {

#ifdef MY_MALLOC_DEBUG
  char buf[256];

  va_list ap;
  va_start(ap,fmt);

  vsnprintf(buf, 255, fmt, ap);

  va_end(ap);

  printf("[%3i] %s",_debug_itr++, buf);
#else
  (void)fmt;
#endif
  return;
}

void *my_malloc(size_t bytes) {

  debug("my_malloc(%i)\n", (int)bytes);

  if (!_session && create_session() < 0) {
    return NULL;
  }

  /* Round up to nearest multiple of 4. */
  size_t cnt = (((bytes-1)>>2)<<2) + 4;

  chunk *c = find_free_chunk( cnt );

  if ( !c ) {
    void *ptr;
    if ( (ptr = sbrk(sizeof(chunk) + cnt)) == (void*)(-1) ) {
      fprintf(stderr, "Couldn't allocate memory!!\n");
      return NULL;
    }

    c = (chunk*)ptr;
    c->_chunk_sz = cnt;
    c->free = NOT_FREE;

    c->prev = _session->_last_chunk;
    c->next = NULL;

    c->prev_free = NULL;
    c->next_free = NULL;

    if ( !_session->_first_chunk )
      _session->_first_chunk = c;

    if ( _session->_last_chunk )
      _session->_last_chunk->next = c;

    _session->_last_chunk = c;

    _session->_chunks_allocated++;

    debug("\tNew chunk at %p\n", c);
    debug("\t_session->_first_chunk = %p\n", _session->_first_chunk);
    debug("\t_session->_last_chunk = %p\n", _session->_last_chunk);
    debug("\tcurrent program break: %p\n", sbrk(0));

  } else {
    c->free = NOT_FREE;

    if (c->next_free && c->prev_free) {
      c->next_free->prev_free = c->prev_free;
      c->prev_free->next_free = c->next_free;

    } else if (c->next_free) {
      c->next_free->prev_free = NULL;

    } else if (c->prev_free) {
      c->prev_free->next_free = NULL;
    }

    if (_session->_first_free_chunk == c)
      _session->_first_free_chunk = c->next_free;

    if (_session->_last_free_chunk == c)
      _session->_last_free_chunk = c->prev_free;

    if (c->_chunk_sz >= cnt + (sizeof(int) + sizeof(chunk) )) {
      chunk *d = (chunk*)((char*)c + sizeof(chunk) + c->_chunk_sz);

      size_t extra = c->_chunk_sz - cnt - sizeof(chunk);
      c->_chunk_sz = cnt;

      d->_chunk_sz = extra;
      d->free = FREE;

      if ( !_session->_first_free_chunk )
        _session->_first_free_chunk = d;

      if ( _session->_last_free_chunk )
        _session->_last_free_chunk->next_free = d;

      d->prev_free = _session->_last_free_chunk;
      _session->_last_free_chunk = d;
      d->next_free = NULL;

      d->next = c->next;
      c->next = d;
      d->prev = c;

      _session->_chunks_allocated++;
    }
  }
  debug("\t_session->_first_free_chunk = %p\n", _session->_first_free_chunk);
  debug("\t_session->_last_free_chunk = %p\n", _session->_last_free_chunk);

  /* return `c` offset by the size of a chunk,
     resulting in a pointer pointing to the
     beginning of user data space*/
  return (sizeof(chunk) + (char*)c);
}

void *my_calloc(size_t nmemb, size_t size) {

  debug("my_calloc(%i, %i)\n", (int)nmemb, (int)size);

  int *mem = (int*)my_malloc(nmemb * size);

  int i = 0, n = nmemb*(size/4);
  for (; i < n; i++) mem[i] = 0;

  return (void*)mem;
}

void *my_realloc(void *ptr, size_t size) {

  debug("my_realloc(%p, %i)\n", ptr, (int)size);

  if ( !ptr ) {
    return my_malloc(size);
  } else if (size == 0) {
    my_free(ptr);
    return NULL;
  }

  chunk *c = (chunk*)((char*)ptr - sizeof(chunk) );

  if ( c < _session->_first_chunk || c > _session->_last_chunk + _session->_last_chunk->_chunk_sz ) {
    fprintf(stderr, "%p is not a reallocatable memory space.\n", ptr);
    return NULL;
  }

  if (size > c->_chunk_sz) {
    if (c->next->free == FREE && c->next->_chunk_sz + sizeof(chunk) + c->_chunk_sz >= size) {
      chunk *n, *p, *c2;
      c2 = c->next;
      n = c2->next;
      p = c2->prev;

      p->next = n;
      if (n) n->prev = p;

      if (_session->_last_chunk == c2)
        _session->_last_chunk = c2->prev;

      n = c2->next_free;
      p = c2->prev_free;

      p->next_free = n;
      if (n) n->prev_free = p;

      if (_session->_last_free_chunk == c2)
        _session->_last_free_chunk = c2->prev_free;

      if (_session->_first_free_chunk == c2)
        _session->_first_free_chunk = c2->next_free;

      c->_chunk_sz += c2->_chunk_sz + sizeof(chunk);

      _session->_chunks_allocated--;
    } else {

      int *new_mem = my_malloc( size );

      if (!new_mem) return NULL; 
      int i = 0, n = c->_chunk_sz / sizeof(int);

      for (; i < n; i++) new_mem[i] = ((int*)ptr)[i];

      my_free(ptr);
      ptr = (void*)new_mem;
    }
  }

  return ptr;
}

void my_free(void *ptr) {

  debug("my_free(%p)\n", ptr);

  chunk *c = (chunk*)((char*)ptr - sizeof(chunk) );

  if ( c < _session->_first_chunk || c > _session->_last_chunk + _session->_last_chunk->_chunk_sz) {
    fprintf(stderr, "[!] Invalid Free! %p is out of range!\n", ptr);
    return;
  }

  debug("\tchunk @ %p; size: %i B\n", c, (int)(c->_chunk_sz) + sizeof(chunk));

  if (c->free == FREE)
    return;

  c->free = FREE;

  if ( !_session->_first_free_chunk ) {

    _session->_first_free_chunk = c;
    _session->_last_free_chunk = c;

    c->prev_free = NULL;
    c->next_free = NULL;

  } else {
    if (_session->_last_free_chunk)
      _session->_last_free_chunk->next_free = c;

    c->prev_free = _session->_last_free_chunk;

    _session->_last_free_chunk = c;
  } 

  if (c->next != NULL && c->next->free == FREE)
    merge_chunks(c, c->next);

  if (c->prev != NULL && c->prev->free == FREE)
    merge_chunks(c->prev, c);
}

int create_session() {

  debug("create_session()\n");

  void *ptr;

  if ( (ptr = sbrk(sizeof(mem_session) )) == (void*)(-1) ) {
    fprintf(stderr, "Failed to start memory session!\n");
    return -1;
  }

  _session = (mem_session*)ptr;

  _session->_first_chunk = NULL;
  _session->_last_chunk = NULL;

  _session->_first_free_chunk = NULL;
  _session->_last_free_chunk = NULL;

  _session->_chunks_allocated = 0;

  return 0;
}

chunk *find_free_chunk(size_t bytes) {

  chunk *curr = _session->_first_free_chunk;

  while(curr != NULL && (curr->_chunk_sz < bytes || curr->free == NOT_FREE)) {
    curr = curr->next_free;
  }

  debug("\tfind_free_chunk(%i) --> %p\n", (int)bytes, curr);
  return curr;
}

void merge_chunks(chunk *a, chunk *b) {

  debug("merge_chunks(%p, %p)\n", a, b);

  a->next_free = b->next_free;
  if (b->next_free) b->next_free->prev_free = a;

  a->next = b->next;
  if (b->next) b->next->prev = a;

  if (_session->_last_free_chunk == b)
    _session->_last_free_chunk = a;

  if (_session->_last_chunk == b)
    _session->_last_chunk = a;

  a->_chunk_sz += b->_chunk_sz + sizeof(chunk);

  _session->_chunks_allocated--;
}
