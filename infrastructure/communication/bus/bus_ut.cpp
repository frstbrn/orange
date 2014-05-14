//
//  bus_ut.cpp
//  orange
//
//  Created by f on 14-4-13.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <iostream>
#include "common/basic/ast.h"
#include "mem_q.h"
#include "shm_pool.h"
#include "shm_q.h"

using namespace std;
using namespace matrix;

u32 dequeue(mem_Q& q)
{
    char buf[1<<20];
    u32 data_len = 0;
    int ret = q.dequeue(buf, 1<<20, data_len);
    if (ret != 0)
    {
        cerr<< "de error[" << ret << "]" << endl;
        return 0;
    }
    else
    {
        printf("de           [%8u]\n", data_len);
    }
    return data_len;
}


u32 enqueue(mem_Q& q)
{
    char buf[1<<20];
    u32 data_len = u32(rand()) % (1<<18);
    int ret = q.enqueue(buf, data_len);
    if (ret != 0)
    {
        cerr<< "en error[" << ret << "]" << endl;
        return 0;
    }
    else
    {
        printf("en [%8u]\n", data_len);
    }
    return data_len;
}

void test_mem_q(u32 times)
{
    //  create
    char* buf = new char[1<<20];
    mem_Q q;
    q.init_attach(buf);
    q.init_clear(1<<20);
    
    //  init rand
    struct timeval tv;
    gettimeofday(&tv, 0);
    srand(tv.tv_usec);
    
    //  test
    u64 sum_en = 0, sum_de = 0;
    for (u32 i = 0; i < times; ++i)
    {
        if(rand() % 2)
        {
            sum_en += enqueue(q);
        }
        else
        {
            sum_de += dequeue(q);
        }
        if (i % 100 == 0)
        {
            printf("en[%8llu] - de[%8llu]\n", sum_en, sum_de);
        }
    }
    
    for (u32 i = 0; i < 100; ++i)
    {
        sum_de += dequeue(q);
    }
    printf("en[%8llu] - de[%8llu]\n", sum_en, sum_de);
    return ;
}

////////////////////////////////////////////////////////////////////////////////

shm_pool* create_shm_pool(u32 times)
{
    list<shm_key_desc> key_list;
    key_t key = 10000;
    for(u32 i = 0; i < times; ++i, ++key)
    {
        shm_key_desc desc;
        desc._key = key;
        desc._size = 1<<16;
        key_list.push_back(desc);
    }
    
    shm_pool* sp = new shm_pool;
    sp->load_shm_into_seg(key_list, 1<<14);
    return sp;
}

void test_shm_pool(u32 times)
{
    shm_pool& sp = *create_shm_pool(times);
    
    //
    for(int i = 0; ;++i)
    {
        matrix::mem_handle handle;
        int ret = sp.alloc_handle(1<<14, handle);
        if (ret == -ENOMEM)
        {
            printf("alloc out: [%u]\n", i);
            break;
        }
        ast(handle._type == E_MEM_DESC_SHM_SEG);    //  good job!
        shm_seg& seg = handle._shm_seg;
        printf("alloc shm handle[%8u] - [%8d] - [%8u] - [%8u] - [%8u]\n"
                , i, seg._shm_key, seg._shm_size, seg._seg_offset, seg._seg_len);
    }
}

void test_shm_q(u32 times)
{
    shmq_pool pool;
    shm_pool& sp = *create_shm_pool(times);
    pool.set_shm_pool(&sp);
    
    list<mem_handle> handle_list;
    
    for(int i = 0; ;++i)
    {
        //  construct pair
        shmq_usage us;
        us._from = i*2;
        us._to = i*2+1;
        us._size = 1<<14;
        us._extra = i;
        
        mem_handle h;
        int ret = pool.alloc_shmq(us, h);
        if (ret == -ENOMEM)
        {
            printf("alloc out: [%u]\n", i);
            sleep(3);
            break;
        }

        ast(ret == 0);
        handle_list.push_back(h);
        
        indented_loop_Q q;
        ret = pool.get_shmq(h, q);
        ast(ret == 0);
        for(int k = 0;;++k)
        {
            ret = q.enqueue((const char*)&us, sizeof(us));
            if(ret == -ENOMEM)
            {
                printf("enqueue [%d] times\n", k);
                break;
            }
            ast(ret == 0);
        }
        
        shm_seg& seg = h._shm_seg;
        printf("alloc shm-seg [%8u] - [%8d] - [%8u] - [%8u] - [%8u]\n"
               , i, seg._shm_key, seg._shm_size, seg._seg_offset, seg._seg_len);
    }
    
    int j = 0;
    for (list<mem_handle>::iterator it = handle_list.begin()
         ; it != handle_list.end()
         ; ++it, ++j)
    {
        indented_loop_Q q;
        pool.get_shmq(*it, q);
        
        char buf[64];
        u32 len = 0;
        for(int k = 0;;++k)
        {
            int ret = q.dequeue(buf, 64, len);
            if(ret == -EAGAIN)
            {
                printf("dequeue [%d] times\n", k);
                break;
            }
            ast(ret == 0);
            ast(len == sizeof(shmq_usage));
        }
        
        shmq_usage& us = *(shmq_usage*)buf;
        printf("dequeue shm-usage:[%8d] - [%8llu] - [%8llu] - [%8u] - [%8llu]\n"
               , j, us._from, us._to, us._size, us._extra);
    }
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char**argv)
{
    u32 times = 100;
    if (argc >= 2)
    {
        times = atol(argv[1]);
    }
    
    printf("run %u times\n", times);
    //test_mem_q(times);
    //test_shm_pool(times);
    test_shm_q(times);
    return 0;
}
