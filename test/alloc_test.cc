#include <gtest/gtest.h>
#include "include/alloc.h"
#include <iostream>

TEST(basic_test, basic_type){
    zzalloc::SimpleFreeListAllocator allocator;
    uint32_t *a = reinterpret_cast<uint32_t*>(allocator.Allocate(sizeof(uint32_t))); //uintptr_t used for converting address to int.
    *a = 0xFFFFFFFF;
    float *f = reinterpret_cast<float*>(allocator.Allocate(sizeof(float)));
    *f = 99.12343429;
    char* c = reinterpret_cast<char*>(allocator.Allocate(sizeof(char)));
    *c = 'a';
    double *d = reinterpret_cast<double*>(allocator.Allocate(sizeof(double)));
    *d = 4503599627370496;
    EXPECT_EQ(*a,0xFFFFFFFF);
    EXPECT_FLOAT_EQ(*f,99.12343429);
    EXPECT_EQ(*c,'a');
    EXPECT_DOUBLE_EQ(*d,4503599627370496);
    EXPECT_EQ(sizeof(a), 8);
    EXPECT_EQ(sizeof(f), 8);
    EXPECT_EQ(sizeof(c), 8);
    EXPECT_EQ(sizeof(d), 8);
    EXPECT_EQ(allocator.getTotalUsage(), 8+8+8+8);
    allocator.Deallocate(a,sizeof(a));
    EXPECT_EQ(allocator.getTotalUsage(), 8+8+8);


}

TEST(basic_test, array_and_llist){

}

TEST(basic, AlignedAlloc) {
  zzalloc::SimpleFreeListAllocator allocator;
  EXPECT_EQ(reinterpret_cast<uintptr_t>(allocator.Allocate(64)) % 8, 0);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(allocator.Allocate(7)) % 8, 0);
  EXPECT_EQ(reinterpret_cast<uintptr_t>(allocator.Allocate(128)) % 8, 0);
}