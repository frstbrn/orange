//
//  shm_pool.cpp
//  orange
//
//  Created by f on 14-4-19.
//  Copyright (c) 2014年 aurora. All rights reserved.
//
#include <cstring>
#include "common/basic/ast.h"
#include "common/os/shm.h"
#include "shm_pool.h"

using namespace std;
using namespace matrix;

////////////////////////////////////////////////////////////////////////////////

open_shm_pool::~open_shm_pool()
{
    for (map<key_t, open_shm_desc>::iterator it = _map.begin()
         ; it != _map.end()
         ; ++it)
    {
        shm_detach(it->second._mem);
    }
    _map.clear();
}

char* open_shm_pool::get_shm(key_t key, u32 size)
{
    //  existed
    for (map<key_t, open_shm_desc>::iterator it = _map.begin()
         ; it != _map.end()
         ; ++it)
    {
        if (it->first == key)
        {
            return it->second._mem;
        }
    }
    
    //  open
    open_shm_desc desc(key, size, -1);
    int ret = shm_create(desc._shm_id, key, size, false);
    ast(ret && desc._shm_id >= 0);
    
    desc._mem = (char*)shm_attach(desc._shm_id, size);
    ast(desc._mem);
    
    _map[key] = desc;
    
    return desc._mem;
}

////////////////////////////////////////////////////////////////////////////////

int shm_pool::load_shm_into_seg(const std::list<shm_key_desc>& shm, u32 seg_size)
{
    if (_seg_size != 0)
    {
        ast(seg_size == _seg_size);
    }
    
    for (list<shm_key_desc>::const_iterator it = shm.begin()
         ; it != shm.end()
         ; ++it)
    {
        //  valid configure data, skip temp
        
        //  cut and insert
        u32 pcs = it->_size / seg_size;
        for(u32 i = 0; i < pcs; ++i)
        {
            shm_seg_desc seg;
            seg._shm_key = it ->_key;
            seg._shm_size = it->_size;
            seg._seg_offset = seg_size*i;
            seg._seg_len = seg_size;
            _seg_pool.add(seg);
            
            printf("_seg_pool.add[%8u] - [%8d] - [%8u] - [%8u] - [%8u]\n"
                   , i, seg._shm_key, seg._shm_size, seg._seg_offset, seg._seg_len);
        }
    }
    
    _seg_size = seg_size;   //  whatever _seg_size == 0
    return 0;
}

int shm_pool::alloc_desc(u32 size, matrix::mem_desc &desc)
{
    matrix::shm_seg_desc seg;
    int ret = _seg_pool.alloc(seg);
    if(ret) return ret;
    
    printf("_seg_pool.alloc[%8d] - [%8u] - [%8u] - [%8u]\n"
           , seg._shm_key, seg._shm_size, seg._seg_offset, seg._seg_len);
    
    desc._type = E_MEM_DESC_SHM_SEG;
    memcpy(desc._buf, &seg, sizeof(shm_seg_desc));
    
    return 0;
}

int shm_pool::free_desc(const mem_desc& desc)
{
    if (desc._type != E_MEM_DESC_SHM_SEG)
        return -1;
    
    const shm_seg_desc& sd = *(shm_seg_desc*)(desc._buf);
    return _seg_pool.free(sd);
}

int shm_pool::get_ptr(const matrix::mem_desc &desc, char *&mem)
{
    shm_seg_desc& sd = *(shm_seg_desc*)desc._buf;
    char* shm = _oshmp.get_shm(sd._shm_key, sd._shm_size);
    ast(shm);
    mem = shm + sd._seg_offset;
    return 0;
}




