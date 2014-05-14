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
#include "common/basic/ast.h"
#include "common/os/shm.h"

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
    
    //
    //  同一块内存可以被反复使用的mem-pool
    //  分配过程分为两次，一次是分配mem的描述，一次是根据描述得到mem的ptr
    //  对于初始分配者，分配得到一个描述并得到ptr，对于attacher，根据别人的desc，得到ptr
    //
    
    struct mem_handle;
    
    struct shared_mpool
    {
        virtual ~shared_mpool(){}
        virtual int alloc_handle(u32 size, mem_handle& desc) = 0;  //  here we don't care fixed-size or not
        virtual int free_handle(const mem_handle& handle) = 0;
        virtual int get_ptr(const mem_handle& handle, char* &mem) = 0;  //  handle can be shared
    };
    
    
    ////////////////////////////////////////////////////////////////////////////////
    
    //  maybe you think it should not be here, but i like, so what?
    
    struct shm_seg
    {
        key_t _shm_key;
        u32 _shm_size;
        u32 _seg_offset;
        u32 _seg_len;
        
        bool operator == (const shm_seg& r) const
        {
            return (_shm_key == r._shm_key
                    && _shm_size == r._shm_size
                    && _seg_offset == r._seg_offset
                    && _seg_len == r._seg_len);
        }
    };
    
    enum
    {
        C_MEM_DESC_MAX_SIZE = 64,
        E_MEM_DESC_SHM_SEG = 1, //  type define
    };
    
    struct mem_handle
    {
        u32 _type;
        union
        {
            shm_seg _shm_seg;
            char _buf[C_MEM_DESC_MAX_SIZE];
        };
        
        
        mem_handle():_type(INV32){}
        mem_handle(const mem_handle& r){*this = r;}
        mem_handle& operator = (const mem_handle& r)
        {
            _type = r._type;
            if(_type == E_MEM_DESC_SHM_SEG)
            {
                _shm_seg = r._shm_seg;
            }
            else
            {
                ast(false);
            }
            return * this;
        }
        
        bool operator == (const mem_handle& r) const
        {
            if(_type != r._type) return false;
            
            if(_type == E_MEM_DESC_SHM_SEG)
            {
                return _shm_seg == r._shm_seg;
            }
            
            return false;
        }
    };
}

#endif
