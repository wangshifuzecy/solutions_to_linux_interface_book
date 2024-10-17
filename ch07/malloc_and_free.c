// Thanks for https://github.com/ritwickdey/malloc-impl-demo.git
// It help me resolve the main problem of malloc and free implementation.
// I mainly modify the free function and scan function.

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>

#define PAGE_SIZE sysconf(_SC_PAGESIZE)
// allocate function allocate mulitple of PAGE_SIZE or MEM_ALLOC_SIZE.
#define MEM_ALLOC_SIZE (PAGE_SIZE)
// if the free space in the end of list >= MEM_DEALLOC_SIZE, then release them.
#define MEM_DEALLOC_SIZE (4 * PAGE_SIZE) 
#define HEADER_SIZE (sizeof(m_block))
// |--------------------|--------------------|-------------------|
// |     m_block       |    Usable Memory    |     m_block       |
// |    (Header)       |       (Data)        |    (Header)       |
// |--------------------|--------------------|-------------------|
// |<--- HEADER_AREA(p) |<--- WRITABLE_AREA(p) 
#define WRITABLE_AREA(p) (((m_block *)p) + 1)
#define HEADER_AREA(p) (((m_block *)p) - 1)
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Uncomment the following line to enable debug mode
#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) debug_printf(fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)  // No operation
#endif

typedef enum { FALSE, TRUE } Boolean;

// I don't use printf which uses `mlloc()` internally
void debug_printf(const char *format, ...){
  va_list argList;
  char buffer[1024];  // Buffer to hold the formatted string
  // Initialize the argument list
  va_start(argList, format);
  // Format the string into the buffer
  vsnprintf(buffer, 1024, format, argList);
  // Write the buffer to stdout using write()
  write(STDOUT_FILENO, buffer, strlen(buffer)); 
  // Clean up the argument list
  va_end(argList);
}

// 32 bytes
typedef struct m_block {
  size_t size;
  Boolean free;
  struct m_block *prev;
  struct m_block *next;
} m_block;

m_block *first_block = NULL;

void add_block(m_block *new_block) {
  if (first_block == NULL) {
    first_block = new_block;
    first_block->next = NULL;
    first_block->prev = NULL;
    return;
  }

  m_block *prev_block = first_block;  // new_block's prev block;
  while (prev_block->next != NULL) {
    prev_block = prev_block->next;
  }

  prev_block->next = new_block;
  new_block->prev = prev_block;
  new_block->next = NULL;
}

// choose_last_unfit_free_block is set for marked if we need to choose the last block which is free when the size is not enough
// if we choose the last block, then when we allocate new memory, the block will be set as the last block.
void *scan(size_t size, Boolean* choose_last_unfit_free_block){
  m_block *ret = first_block;
  if(ret == NULL){
    return NULL;
  }
  int i = 1; //for debug, the block number in the list
  //travse util ret->next == NULL (i.e. ret is the end of the list) or find one fir block.
  while(1){
    //for debug
    //print free size in detail 
    DEBUG_PRINT("%dth free block size: %zu, free status\n", i, ret->size, ret->free);

    if(ret->free == TRUE && ret->size >= size){
      DEBUG_PRINT("free block of size %zu found for %zu size in scan\n", ret->size, size);
      return ret;
    }
    if(ret->next){
      ret = ret->next;
      i++; //for debug
    }else{
      break;
    }
  }
  DEBUG_PRINT("no fit block found for %zu size in scan\n", size);

  if(ret->free == TRUE){
    *choose_last_unfit_free_block = TRUE;
    return ret;
  }else{
    return NULL;
  }
}

//if `block` have the free space bigger than one HEADER, should make one new block on the free space.
void resize_block(m_block *block, size_t size){
  if(block->size - size <= HEADER_SIZE){ //if not enough, just return.
    return;
  }

  m_block *new_block = (m_block *)((char *)block + size);
  new_block->size = block->size - size;
  new_block->free = TRUE;
  new_block->next = block->next;
  new_block->prev = block;
  block->size = size;
  block->next = new_block;
  DEBUG_PRINT("resize %zu bytes block into %zu and %zu\n", size+new_block->size, size, new_block->size);
}

void *allocate(size_t size){
  DEBUG_PRINT("begin allocate, break: %10p\n", sbrk(0));
  void *mem = sbrk(size);
  if(mem == (void*) -1){
    return NULL;
  }
  DEBUG_PRINT("allocated %zu bytes memory\n", size);
  DEBUG_PRINT("after allocate, break: %10p\n", sbrk(0));
  m_block *block = mem;
  block->size = size;
  block->free = TRUE;
  block->next = NULL;
  block->prev = NULL;
  return block;
}

//free and merge free block
void free_and_merge(m_block* block){
  // free_and_merge() called in my_free(), I check the argument in my_free()
  block->free = TRUE;
  DEBUG_PRINT("free %zu bytes %10p\n", block->size, (void*)block);//?

  if(block->prev && block->prev->free == TRUE){
    DEBUG_PRINT("free and merge: prev block is free, size: %zu\n", block->prev->size);
    block->prev->size += block->size;
    block->prev->next = block->next;
    if(block->next){
      block->next->prev = block->prev;
    }
    block = block->prev;
  }
  if(block->next && block->next->free == TRUE){
    DEBUG_PRINT("free and merge: next block is free, size: %zu\n", block->next->size);
    block->size += block->next->size;
    block->next = block->next->next;
  }
}

//check if free space >= MEM_DEALLOC_SIZE,
// if need, then release them
void release_resource_if_need(){
  m_block *ptr = first_block;
  m_block *first_free_block = NULL;
  size_t total_free = 0;
  while(ptr){
    if(ptr->free){
      first_free_block = ptr;
      total_free += ptr->size;
    }else{
      first_free_block = NULL;
      total_free = 0;
    }
    ptr = ptr->next;
  }
  if(total_free < MEM_DEALLOC_SIZE){
    return;
  }
  void *reset = first_free_block;
  if(first_free_block == first_block){
    first_block = NULL;
  }else{
    first_free_block->prev->next = NULL;
  }
  DEBUG_PRINT("important!: before release, break: %10p\n", sbrk(0));
  brk(reset);
  DEBUG_PRINT("important!: released %zu bytes\n", total_free);
  DEBUG_PRINT("important!: after release, break: %10p\n", sbrk(0));
}

void *my_malloc(size_t size){
  /*
    malloc(0) will return either "a null pointer or a
    unique pointer that can be successfully passed to free().
  */
  if(size == 0){
    return NULL;
  }
  m_block *mem = NULL;
  size_t msize = size + HEADER_SIZE;
  
  Boolean choose_last_unfit_free_block = FALSE;
  mem = scan(msize, &choose_last_unfit_free_block); // scan for a fit space
  if(mem == NULL){ // if don't find any fit space
    //allocate size: Multiple of PAGE_SIZE, MEM_ALLOC_SIZE
    size_t allocate_size = MAX(((msize / PAGE_SIZE) + 1) * PAGE_SIZE,MEM_ALLOC_SIZE);
    mem = allocate(allocate_size);
    if(mem == NULL){//fail to allocate, set errno and return NULL;
      errno = ENOMEM;
      return NULL;
    }
    add_block(mem);
  }else if(choose_last_unfit_free_block == TRUE){
    size_t allocate_size = MAX((((msize - mem->size)/ PAGE_SIZE) + 1) * PAGE_SIZE,MEM_ALLOC_SIZE);
    DEBUG_PRINT("choose last unfit free block, and allocate size: %zu\n", allocate_size);
    allocate(allocate_size);
    if(mem == NULL){//fail to allocate, set errno and return NULL;
      errno = ENOMEM;
      return NULL;
    }
    mem->size += allocate_size;
    resize_block(mem, msize);
    mem->free = FALSE;
    return WRITABLE_AREA(mem);
  }
  //allocate mimuinm size may be bigger than msize
  resize_block(mem, msize);
  mem->free = FALSE;
  return WRITABLE_AREA(mem);
}

void my_free(void *mem){
  if(mem == NULL){
    return;
  }
  m_block *block = HEADER_AREA(mem);
  free_and_merge(block);

  // can be added for release a lot of free space ( >= MEM_DEALLOC_SIZE)
  release_resource_if_need(); 
}

// check for malloc and free
void check_malloc_free(){
  char *a = my_malloc(8000);
  assert(a != NULL);
  debug_printf("------\n");
  char *b = my_malloc(4000);
  assert(b != NULL);
  debug_printf("------\n");
  my_free(a);
  debug_printf("------\n");
  char *c = my_malloc(4000);
  assert(c != NULL);
  debug_printf("------\n");
  char *d = my_malloc(4128);
  assert(d != NULL);
  debug_printf("------\n");
  my_free(b);
  debug_printf("------\n");
  my_free(c);
  debug_printf("------\n");
  my_free(d);
}

// check release_resource_if_need()
void check_release(){
  char *a = my_malloc(8000);
  char *b = my_malloc(9000);
  char *c = my_malloc(10000);
  my_free(a);
  my_free(b);
  my_free(c);
}

// check free_and_merge()
void check_free_merge(){
  char *a = my_malloc(400);
  char *b = my_malloc(4000);
  char *c = my_malloc(3500);
  my_free(a);
  my_free(b);
  my_free(c);
  char *d = my_malloc(8000);
  my_free(d);
}

int main(){
  DEBUG_PRINT("init break: %10p\n\n", sbrk(0));
  // check_malloc_free();
  check_release();
  // check_free_merge();
}