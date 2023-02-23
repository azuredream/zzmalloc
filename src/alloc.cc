//implementation of alloc.h
//step1. include .h and libs
#include "alloc.h"
#include <iostream>
#include <cassert>


//step2. set namespace
namespace zzalloc{
    //step3. implement functions


    SimpleFreeListAllocator::~SimpleFreeListAllocator(){
        FreeListNode* cur = FreeLists[0];
        while(cur){
            FreeListNode* next = cur->next;
            free(cur); //no delete
            cur = next;
        }

        if(pool_start){
            free(pool_start);
        }
    }

    void* Allocate(int32_t objsize){

        assert(objsize>0);
        //large object goes to malloc
        if (objsize > kMaxSmallObjSize) {
            totaluseage.fetch_add(objsize, std::memory_order_relaxed);
            return (char*)malloc(objsize);
        }
        //find freelist
        FreeListNode* cur_freelist = FreeLists[LocateFreeList(objsize)];
        if (unlikely(cur_freelist == nullptr)) { //unlikely
            //slow path

            LocateBlocksFromPool(objsize);
            //TODO: what if locate failed? 
        }
        //fast path
        //return the first block on the freelist
        FreeListNode* to_return = cur_freelist;
        cur_freelist = cur_freelist->next;
        return to_return; 
    }

    //pagesize: 8,16,24,...
    //index:    0,1 ,2 ,...
    int32_t LocateFreeList(int32_t objsize) {
        return (objsize + kBlockUnit-1)/kBlockUnit - 1;
    }
    
    void LocateBlocksFromPool(int32_t objsize){

        //less than one block space in pool: enlarge space
        uint32_t pool_size = pool_end - pool_start;
        if (pool_size < objsize) {
            //use up pool
            //locate new pool
        }
        
        
        int32_t totalSize = BLOCKS_EACH_LOCATE * objsize;
        if (pool_size < totalSize) {
            //not enough space in pool: locate as many as possible
            totalSize = objsize * (pool_size/objsize);
        } 
        //enough space in pool: locate BLOCKS_EACH_LOCATE blocks
        char* to_return = pool_start;
        pool_start += totalSize;
        totaluseage.fetch_add(totalSize, std::memory_order_relaxed);
        // return to_return;
        //freelist
        FreeListNode* cur_freelist = FreeLists[LocateFreeList(objsize)];
        cur_freelist



    }


}


// SimpleFreeListAlloc::~SimpleFreeListAlloc() {
//   //释放Freelists
//   FreeList* cur = freelist_[0];
//   while (cur) {
//     FreeList* next = cur->next;
//     free(cur);
//     cur = next;
//   }
//   //释放空闲内存池
//   if (free_list_start_pos_) {
//     free(free_list_start_pos_);
//   }
// }


