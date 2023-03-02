#include <gtest/gtest.h>
#include "include/alloc.h"

TEST(basic_test, first_test){
    int units = 1;
    zzalloc::SimpleFreeListAllocator allocator;
    int *a = (int *)allocator.Allocate(sizeof(int) * units);
    a[0] =99;
    std::cout << "a: " << a[0] << "addr:  " << a;
    EXPECT_EQ(a[0],99);

    std::cout << "a: " << a[0] << "addr:  " << a;
    allocator.Deallocate(a, sizeof(int) * units);
    std::cout << "a addr: " << a;
}

TEST(basic, AlignedAlloc) {
  zzalloc::SimpleFreeListAllocator allocator;
  EXPECT_EQ(reinterpret_cast<uintptr_t>(allocator.Allocate(64)) % 8, 0);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(allocator.Allocate(7)) % 8, 0);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(allocator.Allocate(128)) % 8, 0);
}