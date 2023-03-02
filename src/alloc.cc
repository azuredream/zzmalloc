//implementation of alloc.h
//step1. include .h and libs
#include "alloc.h"
#include <iostream>
#include <cassert>


//step2. set namespace
namespace zzalloc{
    //step3. implement functions
    SimpleFreeListAllocator::~SimpleFreeListAllocator(){
        // if (FreeLists){
        //     for(int i = 0; i < sizeof(FreeLists)/sizeof(FreeLists[0]); i++) {
        //         FreeListNode** cur = FreeLists + i;
        //         FreeListNode* curnode = *cur;
        //         while(curnode){
        //             FreeListNode* nextnode = curnode->next;
        //             free(curnode);
        //             curnode = nextnode;
        //         }
        //     }
        //     free(FreeLists);
        // }

        // if(pool_start){
        //     free(pool_start);
        // }
    }

    void* SimpleFreeListAllocator::Allocate(int32_t objsize){
        assert(objsize>0);
        //large object goes to malloc
        if (objsize > kMaxSmallObjSize) {
            totaluseage.fetch_add(objsize, std::memory_order_relaxed);
            return (char*)malloc(objsize);
        }
        //find freelist
        FreeListNode** cur_freelist = FreeLists + LocateFreeList(objsize);
        if (unlikely(*cur_freelist == nullptr)) { //unlikely
            //slow path
            LocateBlocksFromPool(AlignmentHelper(objsize)); //ceiling, block size
            // return (objsize + kBlockUnit-1)/kBlockUnit - 1;
            //TODO: what if locate failed? 
        }
        //fast path
        //return the first block on the freelist
        FreeListNode* to_return = *cur_freelist;
        *cur_freelist = (*cur_freelist)->next;
        to_return->next = nullptr;
        totaluseage.fetch_add(objsize, std::memory_order_relaxed);
        return to_return; 
    }


    void  SimpleFreeListAllocator::Deallocate(void* addr, int32_t objsize){
        if (addr) {
            FreeListNode* p = (FreeListNode*)addr;
            if (objsize > kMaxSmallObjSize) {
                free(p);
                p = nullptr;
                return;
            }

            FreeListNode** cur_freelist = FreeLists + LocateFreeList(objsize);
            if (*cur_freelist == nullptr) {
                (*cur_freelist) = p;
            }else {
                p->next = *cur_freelist;
                (*cur_freelist) = p;
            }
            totaluseage.fetch_sub(objsize, std::memory_order_relaxed);
        }
    }

    //pagesize: 8,16,24,...
    //index:    0,1 ,2 ,...
    int32_t SimpleFreeListAllocator::LocateFreeList(int32_t objsize) {
        assert(objsize>0);
        return (objsize - 1)/kBlockUnit;
        // return (objsize + kBlockUnit-1)/kBlockUnit - 1;
    }

    void SimpleFreeListAllocator::LocateBlocksFromPool(uint32_t blksize){
        assert(blksize>0);
        uint32_t pool_size = pool_end - pool_start;
        int32_t totalSize = BLOCKS_EACH_LOCATE * blksize;

        if (pool_size < blksize) {
            //less than one block space in pool: enlarge space
            //use up pool
            if (pool_size > 0 && pool_size >= kBlockUnit) {
                uint32_t useupsize = (pool_size/kBlockUnit) * kBlockUnit; //floor
                FreeListNode** cur_freelist = FreeLists + LocateFreeList(useupsize);
                if (*cur_freelist == nullptr) {
                    *cur_freelist = (FreeListNode*)pool_start;
                    (*cur_freelist)->next = nullptr;
                } else {
                    ((FreeListNode*)pool_start)->next = *cur_freelist;
                    (*cur_freelist)->next = (FreeListNode*)pool_start;
                }
                pool_start+=useupsize;
                if (pool_start){
                    free(pool_start);
                }
            }
            //locate new pool
            int32_t new_pool_size = totalSize * 2;
            pool_start = (char*)malloc(new_pool_size + AlignmentHelper(heap_size_>> 4)); //why 4? TODO
            pool_end = pool_start + new_pool_size;
        }

        //poolsize >= 1 block from here.
        int32_t blocknum  = BLOCKS_EACH_LOCATE;
        pool_size = pool_end - pool_start;
        if (pool_size < totalSize) {
            //not enough space in pool: locate as many as possible
            blocknum = pool_size / blksize;
            totalSize = blksize * blocknum;
        } 
        //enough space in pool: locate blocknum blocks
        char* toreturn = pool_start;
        pool_start += totalSize;
        totaluseage.fetch_add(totalSize, std::memory_order_relaxed);

        //update freelist
        FreeListNode** cur_freelist = FreeLists + LocateFreeList(blksize);
        assert(*cur_freelist == nullptr);
        *cur_freelist = (FreeListNode*)toreturn;
        FreeListNode* cur = *cur_freelist;

        while(blocknum>1) {
            FreeListNode* nextnode = (FreeListNode*)((char*)cur + blksize); 
            cur->next = nextnode;
            cur = cur->next;
            blocknum--;
        }
        cur->next = nullptr;
    }

    int32_t SimpleFreeListAllocator::AlignmentHelper(int32_t objsize) {
        return (objsize + kBlockUnit - 1) & ~(kBlockUnit - 1);
    }
}