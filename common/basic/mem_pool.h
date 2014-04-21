//
//  mem_pool.h
//  orange
//
//  Created by f on 14-4-13.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_mem_pool_h
#define orange_mem_pool_h

#include "common/basic/bits.h"

namespace matrix {
    
    //
    //  无差别的mempool，一块内存只可以使用一次
    //
    
    struct mem_pool
    {
        virtual ~mem_pool(){}
        virtual int alloc(char* &mem, u32 mem_size) = 0;
        virtual int free(char* mem, u32 mem_size) = 0;
    };
    
////////////////////////////////////////////////////////////////////////////////

    //
    //  同一块内存可以被反复使用的mem-pool
    //  分配过程分为两次，一次是分配mem的描述，一次是根据描述得到mem的ptr
    //  对于初始分配者，分配得到一个描述并得到ptr，对于attacher，根据别人的desc，得到ptr
    //
    
    struct mem_desc
    {
        u32 _type;
        static const u32 C_MEM_DESC_MAX_SIZE = 64;  //  1<<8
        char _buf[C_MEM_DESC_MAX_SIZE];
    };
    
    enum
    {
        E_MEM_DESC_SHM_SEG = 1,
    };
    
    struct desc_mpool
    {
        virtual ~desc_mpool(){}
        virtual int alloc_desc(u32 size, mem_desc& desc) = 0;  //  here we don't care fixed-size or not
        virtual int free_desc(const mem_desc& desc) = 0;
        virtual int get_ptr(const mem_desc& desc, char* &mem) = 0;
    };
}

#endif
