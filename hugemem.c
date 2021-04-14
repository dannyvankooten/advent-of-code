#include "hugemem.h"

const size_t pagesize = 1 << 21; // 2MB

void* hugemem(size_t _sz){
	size_t sz = (_sz + pagesize - 1) & ~(pagesize - 1);
	void *ptr = mmap(NULL, sz, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_POPULATE|MAP_HUGETLB|MAP_HUGE_2MB, 0, 0);
	if (ptr == MAP_FAILED) {
		ptr = NULL;
	}

  return ptr;
}

void hugemem_free(void* ptr, size_t _sz) {
  size_t sz = (_sz + pagesize - 1) & ~(pagesize - 1);
  assert(munmap(ptr, sz) == 0);
}