// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
  char *arr_page_cow;
  char *end_temp;
  uint64 page_count;
} kmem;

/*
int page_sums = 0;

int *page_cow_arr;

void
addCow(int n)
{
  page_cow_arr[n % page_sums]++;
}

int
getCowArr(int n)
{
  return page_cow_arr[n % page_sums];
}

void
unAddCow(int n)
{
  page_cow_arr[n % page_sums]--;
}
*/
uint64
getIndex(uint64 pa)
{
  pa = PGROUNDDOWN(pa);
  uint64 res = (pa - (uint64)kmem.end_temp) / PGSIZE;
  if(res < 0 || res >= kmem.page_count)
  {
    printf("index error res: %p, pa: %p, kmem.end: %p\n", res, pa, kmem.end_temp);
    panic("getIndex");
  }
  return res;
}

void
addCowCount(void *pa)
{
  acquire(&kmem.lock);
  kmem.arr_page_cow[getIndex((uint64)pa)]++;
  release(&kmem.lock);
}

void
unAddCowCount(void *pa)
{
  acquire(&kmem.lock);
  kmem.arr_page_cow[getIndex((uint64)pa)]--;
  release(&kmem.lock);
}

uint64
getCowCount(void *pa)
{
  acquire(&kmem.lock);
  uint64 res = kmem.arr_page_cow[getIndex((uint64)pa)];
  release(&kmem.lock);
  return res;
}

void
getPageCount(void *pa_start, void *pa_end)
{
  char *p = (char*)PGROUNDUP((uint64) pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p = p + PGSIZE)
  {
    kmem.page_count++;
  }
}

void 
printPageCount()
{
  int count = 0;
  struct run *r;
  acquire(&kmem.lock);
  r = kmem.freelist;
  while(r->next)
  {
    count++;
    r = r->next;
  }
  release(&kmem.lock);
  printf("free page count is %d\n", count);
}

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  kmem.page_count = 0;
  getPageCount(end, (void*) PHYSTOP);

  printf("all page_count is: %d\n", kmem.page_count);

  acquire(&kmem.lock);

  kmem.arr_page_cow = end;
  for(int i = 0; i < kmem.page_count; i++)
  {
    kmem.arr_page_cow[i] = 0;
  }
  kmem.end_temp = kmem.arr_page_cow + kmem.page_count;

  release(&kmem.lock);

  freerange(kmem.end_temp, (void*)PHYSTOP);
  /* 

  acquire(&kmem.lock);

  struct run *r = kmem.freelist;
  int i = 0;
  while(r)
  {
    i++;
    r = r->next;
  }
  page_sums = i;

  int *arr_temp = (int*)malloc(page_sums * sizeof(int));
  for(i = 0; i < page_sums; i++)
  {
    arr_temp[i] = 0;
  }
  page_cow_arr = arr_temp;

  release(&kmem.lock);
  */
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  if(kmem.arr_page_cow[getIndex((uint64)pa)] > 1)//has other process use
  {
    printf("count--\n");
    unAddCowCount((void*)pa);
    return;
  }
  if(kmem.arr_page_cow[getIndex((uint64)pa)] == 1)//only parent process use
  {
    printf("free page\n");
    unAddCowCount((void*)pa);
  }

  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  printf("into kalloc\n");
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
  {
    memset((char*)r, 5, PGSIZE); // fill with junk
    //printPageCount();
    addCowCount(r);
  }
  
  return (void*)r;
}

