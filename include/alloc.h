// This allocator optimizes the memory allocation/deallocation of small objects (<=4kb)
// Not threadsafe

// pool_start, pool_end
// FreeList[sizebucket][FreeList linkedlist]

// Allocate(n)
//     if n > 4kb:
//         return malloc(n)
//     get sizebucket
//     blist = FreeList[sizebucket]
//     if not blist:
//         locate 10 blocks of size n from memory pool
//     adjust blisthead
//     return the first block on blist

// Deallocate(addr, size)
//     if n > 4kb:
//         free(addr)
//     get sizebucket
//     blist = FreeList[sizebucket]
//     put block to blisthead

#ifndef ZZALLOC_H_
#define ZZALLOC_H_
#include <cstdint>
#include <atomic>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define BLOCKS_EACH_LOCATE 10
namespace zzalloc{

//The memory node can be either on the FreeList or in use.
//saves 8 bytes by using union
union FreeListNode{
    union FreeListNode* next;
    char data[1]; //why?
};

class SimpleFreeListAllocator final {
    public:
    SimpleFreeListAllocator() = default;
    ~SimpleFreeListAllocator();
    void* Allocate(int32_t objsize); //unsigned int causes problems in calculation
    void  Deallocate(void* addr, int32_t objsize);
    uint32_t getTotalUsage(); //unsigned int is for memory represent.

    private:
    //pool
    char* pool_start = nullptr;
    char* pool_end   = nullptr;
    //FreeLists
    static constexpr uint32_t kMaxSmallObjSize = 4096; //Obj larger than this goes to malloc
    static constexpr uint32_t kBlockUnit = 8; //the size of freelistnodes must be a multiple of blockUnit// alignment
    static constexpr uint32_t kFreeListsSize = kMaxSmallObjSize/kBlockUnit;
    FreeListNode* FreeLists[kFreeListsSize] = {nullptr}; //[8bytellist, 16bytellist, 24bytellist,... 4096bytellist]

    //stat
    //?? for each allocator? depends on how to use. 
    //If each allocator for each thread, no atomic is needed.
    //global memory usage
    std::atomic<uint32_t> totaluseage;
    //thread memory usage
    int32_t heap_size_ = 0;


    //helper functions
    private:
    int32_t AlignmentHelper(int32_t objsize);
    void LocateBlocksFromPool(uint32_t blocksize);
    int32_t LocateFreeList(int32_t objsize);
};
}


#endif