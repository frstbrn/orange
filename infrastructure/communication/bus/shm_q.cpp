//
//  shm_q.cpp
//  orange_proj
//
//  Created by f on 14-5-11.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#include "shm_q.h"

using namespace matrix;
using namespace std;

int shmq_pool::alloc_shmq(const shmq_usage& usage, mem_handle& handle)
{
    if(find_shmq(usage, handle))
        return -EEXIST;
    
    int ret = _pool->alloc_handle(usage._size, handle);
    if(ret) return ret;
    
    using_shmq us;
    us._usage = usage;
    us._handle = handle;
    
    indented_loop_Q q;
    ret = get_shmq(handle, q);
    if(ret) return ret;
    q.init_clear(usage._size);  //  == handle._shm_seg._seg_len
    
    _using.push_back(us);    //  copy constructor
    return 0;
}

bool shmq_pool::find_shmq(const shmq_usage& usage, mem_handle& handle)
{
    for(std::list<using_shmq>::iterator it = _using.begin()
        ; it != _using.end()
        ; ++it)
    {
        if(it->_usage == usage)
        {
            handle = it->_handle;
            return true;
        }
    }
    return false;
}

int shmq_pool::get_shmq(const mem_handle& handle, indented_loop_Q& q)
{
    char* mem = NULL;
    int ret = _pool->get_ptr(handle, mem);
    if(ret) return ret;
    q.init_attach(mem);
    return 0;
}
