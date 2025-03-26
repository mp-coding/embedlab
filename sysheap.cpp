/**
 * sysheap
 * Created on: 03/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#include "sysheap.hpp"

#include "tlsf.h"
#include "FreeRTOS.h"

#include <osal/critical_section.hpp>

#include <cstdint>
#include <cassert>
#include <cstring>
#include <mutex>

namespace {
#ifndef HEAP_SIZE
#define HEAP_SIZE (1024 * 128)
#endif

    tlsf_t                 heap {};
    std::uint8_t           raw_mem_heap[HEAP_SIZE] {};
    osal::critical_section lock;
} // namespace

extern "C" {

void sysheap_init()
{
    heap = tlsf_create_with_pool(raw_mem_heap, sizeof raw_mem_heap);
    assert(heap != nullptr);
}

void* _malloc_r(struct _reent* re, const size_t size)
{
    std::lock_guard _lock {lock};

    void* p = tlsf_malloc(heap, size);
    if (p == nullptr) { re->_errno = ENOMEM; }
    return p;
}

void _free_r(struct _reent* re, void* mem)
{
    if (mem == nullptr) {
        re->_errno = EINVAL;
        return;
    }

    std::lock_guard _lock {lock};
    tlsf_free(heap, mem);
}

void* _calloc_r(struct _reent* re, const size_t nitems, const size_t size)
{
    std::lock_guard _lock {lock};

    void* p = tlsf_malloc(heap, nitems * size);
    if (p == nullptr) {
        re->_errno = ENOMEM;
        return nullptr;
    }

    memset(p, 0, nitems * size);
    return p;
}

void* _realloc_r(struct _reent* re, void* ptr, const size_t new_size)
{
    std::lock_guard _lock {lock};

    void* p = tlsf_realloc(heap, ptr, new_size);
    if (p == nullptr) {
        re->_errno = ENOMEM;
        return nullptr;
    }
    return p;
}

void* _memalign_r(struct _reent* re, const size_t align, const size_t nbytes)
{
    std::lock_guard _lock {lock};

    void* p = tlsf_memalign(heap, align, nbytes);
    if (p == nullptr) {
        re->_errno = ENOMEM;
        return nullptr;
    }
    return p;
}

void* malloc(const size_t size) { return _malloc_r(_REENT, size); }
void  free(void* mem) { _free_r(_REENT, mem); }
void* calloc(const size_t nitems, const size_t size) { return _calloc_r(_REENT, nitems, size); }
void* realloc(void* ptr, const size_t new_size) { return _realloc_r(_REENT, ptr, new_size); }
void* memalign(const size_t align, const size_t nbytes) { return _memalign_r(_REENT, align, nbytes); }
}