
// #include "alloc.h"
// g++ -c src/alloc.cc -o alloc.o -Iinclude/
// g++ -c src/main.cc -o main.o -Iinclude/
// g++ main.o alloc.o -o main
#include "include/alloc.h"
#include <iostream>
int main(int argc, const char** argv) {
    
    zzalloc::SimpleFreeListAllocator allocator;
    std::cout<<allocator.getTotalUsage();
    uint32_t *a = reinterpret_cast<uint32_t*>(allocator.Allocate(sizeof(uint32_t))); //uintptr_t used for converting address to int.
    *a = 0xFFFFFFFF;
    std::cout<<allocator.getTotalUsage();
    float *f = reinterpret_cast<float*>(allocator.Allocate(sizeof(float)));
    std::cout<<allocator.getTotalUsage();
    *f = 99.12343429;
    char* c = reinterpret_cast<char*>(allocator.Allocate(sizeof(char)));
    std::cout<<allocator.getTotalUsage();
    *c = 'a';
    double *d = reinterpret_cast<double*>(allocator.Allocate(sizeof(double)));
    *d = 4503599627370496;
}
