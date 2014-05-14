//
//  shm_q.h
//  orange
//
//  Created by f on 14-4-11.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_shm_q_h
#define orange_shm_q_h

#include <list>

#include "mem_q.h"
#include "shm_pool.h"

namespace matrix
{
    
    //
    //  assembler, shm_pool poly implement mem_pool
    //  then shm_Q alloc mem from shm_pool and assign it to indented_loop_Q
    //
    
    struct shmq_usage
    {
        u64 _from;
        u64 _to;
        u32 _size;
        u64 _extra;
        
        shmq_usage(){}
        shmq_usage(const shmq_usage& r)
        {
            _from = r._from;
            _to = r._to;
            _size = r._size;
            _extra = r._extra;
        }
        bool operator == (const shmq_usage& r) const
        {
            return _from == r._from
            && _to == r._to
            && _size == r._size
            && _extra == r._extra;
        }
    };
    
    struct using_shmq
    {
        shmq_usage _usage;
        mem_handle _handle;

        using_shmq(){}
        using_shmq(const using_shmq& r){_usage = r._usage; _handle = r._handle;}
        
        bool operator == (const using_shmq& r) const
        {
            return
            _usage == r._usage
            &&
            _handle == r._handle;
        }
    };
    
    struct shmq_pool
    {
        int alloc_shmq(const shmq_usage& usage, mem_handle& handle);
        bool find_shmq(const shmq_usage& usage, mem_handle& handle);
        int recyle_shmq(const mem_handle& handle){return -1;}
        int get_shmq(const mem_handle& handle, indented_loop_Q& q);
        void set_shm_pool(shared_mpool* pool){_pool = pool;}
        
        //  internal usage
        shared_mpool* _pool;
        std::list<using_shmq> _using;  //  should be replaced by shm-based struct
    };

}

#endif
