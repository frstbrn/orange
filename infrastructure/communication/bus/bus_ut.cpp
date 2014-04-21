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

void test_shm_pool(u32 times)
{
    list<shm_key_desc> key_list;
    key_t key = 10000;
    u32 i = 0;
    for(; i < times; ++i, ++key)
    {
        shm_key_desc desc;
        desc._key = key;
        desc._size = 1<<16;
        key_list.push_back(desc);
    }
    
    shm_pool sp;
    sp.load_shm_into_seg(key_list, 1<<14);
    
    //
    i = 0;
    for(; ;++i)
    {
        matrix::mem_desc desc;
        int ret = sp.alloc_desc(1<<14, desc);
        if (ret == -ENOMEM)
        {
            printf("alloc out: [%u]\n", i);
            break;
        }
        ast(desc._type == E_MEM_DESC_SHM_SEG);
        shm_seg_desc& seg = *(shm_seg_desc*)(desc._buf);
        printf("[%8u] - [%8d] - [%8u] - [%8u] - [%8u]\n"
                , i, seg._shm_key, seg._shm_size, seg._seg_offset, seg._seg_len);
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
    test_shm_pool(times);
    return 0;
}
