#pragma once

#include <stddef.h>
#include <iostream>

template <class T>
class CustomAllocator
{
public:
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using value_type = T;

    CustomAllocator()
    {
    }

    CustomAllocator(const CustomAllocator&)
    {
    }

    pointer allocate(size_type n, const void* = 0)
    {
        T* t = (T*)malloc(n * sizeof(T));
        // std::cout << "  used CustomAllocator to allocate   at address " << t << " (+)" << std::endl;
        return t;
    }

    void deallocate(void* p, size_type)
    {
        if (p)
        {
            free(p);
            // std::cout << "  used CustomAllocator to deallocate at address " << p << " (-)" << std::endl;
        }
    }

    pointer address(reference x) const
    {
        return &x;
    }

    const_pointer address(const_reference x) const
    {
        return &x;
    }

    CustomAllocator<T>& operator=(const CustomAllocator&)
    {
        return *this;
    }

    void construct(pointer p, const T& val)
    {
        new ((T*)p) T(val);
    }

    void destroy(pointer p)
    {
        p->~T();
    }

    size_type max_size() const
    {
        return size_t(-1);
    }

    template <class U>
    struct rebind
    {
        typedef CustomAllocator<U> other;
    };

    template <class U>
    CustomAllocator(const CustomAllocator<U>&)
    {
    }

    template <class U>
    CustomAllocator& operator=(const CustomAllocator<U>&)
    {
        return *this;
    }
};