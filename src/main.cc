
// #include "alloc.h"
// g++ -c src/alloc.cc -o alloc.o -Iinclude/
// g++ -c src/main.cc -o main.o -Iinclude/
// g++ main.o alloc.o -o main
#include "include/alloc.h"
#include <iostream>
int main(int argc, const char** argv) {
    
    std::cout << "ready to core\n";
    zzalloc::SimpleFreeListAllocator allocator;
    int *a = (int *)allocator.Allocate(sizeof(int) * 10);
    int *b = (int *)allocator.Allocate(sizeof(int) * 1);
    for(int i = 0; i < 10; i++) {
        a[i] = i*99;
    }
    b[0] =91119;
    std::cout << "!!!!!!!!!!a: " << a[0] << "addr:  " << a;
    // EXPECT_EQ(a[0],99)<< "aaaaaaaaaaaaaa";;

    std::cout << "a: " << a[0] << "addr:  " << a;
    allocator.Deallocate(a, sizeof(int) * 10);
    allocator.Deallocate(b, sizeof(int) * 1);
    std::cout << "a addr: " << a;
}
