#include <jsystem/mem.hpp>
#include <memory.h>
#include <stdlib.h>

// If profiling, we need to see what we allocate. Additionally, this could be used for custom allocations instead.
#ifdef TRACY_ENABLE

void* operator new(std::size_t n)
{
    void* ret = malloc(n);
    TracyAlloc(ret, n);
    return ret;
}

void operator delete(void* p) throw()
{
    TracyFree(p);
    free(p);
}

void* operator new[](std::size_t s)
{
    void* ret = malloc(s);
    TracyAlloc(ret, s);
    return ret;
}
void operator delete[](void* p) throw()
{
    TracyFree(p);
    free(p);
}

void operator delete(void* p, std::size_t)
{
    TracyFree(p);
    free(p);
}

void operator delete[](void* p, std::size_t)
{
    TracyFree(p)
    free(p);
}

#endif