#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>
#include <linux/mman.h>

void* hugemem(size_t _sz);
void hugemem_free(void* ptr, size_t _sz);