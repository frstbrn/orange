//
//  shm_pool.h
//  orange
//
//  Created by f on 14-4-14.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_shm_pool_h
#define orange_shm_pool_h

#include <list>
#include <map>
#include <cerrno>
#include "common/basic/mem_pool.h"

namespace matrix {
    
    //
    //  shm not only used in united on piece
    //  but also could be cut into pieces, for different usage
    //  in one context, one shm should be opened only once for multi usage
    //  so we need a pool, to reuse shm
    //
    
    ////////////////////////////////////////////////////////////////////////////
    
    template<class T>
    struct atomic_pool //  based on value instead of pointer to void allocate problem
    {
        virtual ~atomic_pool(){}
        virtual void add(const T& t){_free.push_back(t);}
        virtual int del(T& t)
        {
            typename std::list<T>::iterator it = _free.begin();
            for(; it != _free.end()
                ; ++it)
            {
                const T& tt = *it;
                if(tt == t)
                    break;
            }

            if(it == _free.end())
                return -1;
            
            _free.erase(it);
            return 0;
        }
        virtual int alloc(T& t)
        {
            typename std::list<T>::iterator it = _free.begin();
            if(it == _free.end())
                return -ENOMEM;
            
            t = *it;
            _used.push_back(*it);
            _free.erase(it);
            return 0;
        }
        virtual int free(const T& t)
        {
            typename std::list<T>::iterator it = _used.begin();
            for(; it != _used.end()
                ; ++it)
            {
                
                const T& tt = *it;
                if(tt == t)
                    break;
            }
            if(it == _used.end())   //  not found
                return -1;
            
            _free.push_back(*it);
            _used.erase(it);
            
            return 0;
        }
        
        std::list<T> _free;
        std::list<T> _used;
    };

    struct open_shm_desc
    {
        open_shm_desc():_key(0), _shm_size(0), _shm_id(-1), _mem(0){}
        open_shm_desc(key_t key, u32 size, int id)
        : _key(key), _shm_size(size), _shm_id(id), _mem(0){}
        
        key_t _key;
        u32 _shm_size;
        int _shm_id;
        char* _mem;
    };
    
    struct open_shm_pool
    {
        ~open_shm_pool();    //  close all shm
        char* get_shm(key_t key, u32 size); //  get if existed, open if not
        std::map<key_t, open_shm_desc> _map;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    
    struct shm_key_desc
    {
        key_t _key;
        u32 _size;
    };

    struct shm_pool : public shared_mpool
    {
        shm_pool():_seg_size(0){}
        virtual ~shm_pool(){}
        int load_shm_into_seg(const std::list<shm_key_desc>& shm, u32 seg_size);
        
        virtual int alloc_handle(u32 size, mem_handle& desc);  //  size for assert only
        virtual int free_handle(const mem_handle& desc);
        virtual int get_ptr(const mem_handle& desc, char* &mem);  //  static implimented, without init
        
        u32 _seg_size;
        atomic_pool<shm_seg> _seg_pool;
        open_shm_pool _oshmp;
    };
    
}

#endif
