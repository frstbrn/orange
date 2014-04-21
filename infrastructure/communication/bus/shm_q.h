//
//  shm_q.h
//  orange
//
//  Created by f on 14-4-11.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_shm_q_h
#define orange_shm_q_h

#include <set>

#include "common/basic/mem_pool.h"
#include "mem_q.h"

namespace matrix {
    
    //
    //  assembler, shm_pool poly implement mem_pool
    //  then shm_Q alloc mem from shm_pool and assign it to indented_loop_Q
    //
    
    struct shm_q_pool
    {
        int alloc_shmq(indented_loop_Q& q);
        int specified_alloc_mq(u32 type, void* spec_data, indented_loop_Q& q);
        int recycle_shmq(indented_loop_Q& q);
        
        //  internal usage
        specified_mpool * _mpool;
        std::set<indented_loop_Q*> _used_q;
    };

}

#endif
