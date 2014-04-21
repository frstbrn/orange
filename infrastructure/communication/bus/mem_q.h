//
//  mem_q.h
//  orange
//
//  Created by f on 14-4-11.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_mem_q_h
#define orange_mem_q_h

#include "common/basic/bits.h"

namespace matrix {
    
    struct loop_Q_head
    {
        u32 _head;     // you could not write any data at head
        u32 _tail;
    };
    
    struct loop_Q
    {
        u32 _buf_size;
        char _buf[0];
        
        void enqueue(u32 offset, const char *data, u32 data_len)
        {
            is_split(offset, data_len)
            ? split_enqueue(offset, data, data_len)
            : unite_enqueue(offset, data, data_len);
        }
        
        void dequeue(u32 offset, char *buf, u32 data_len)
        {
            is_split(offset, data_len)
            ? split_dequeue(offset, buf, data_len)
            : unite_dequeue(offset, buf, data_len);
        }
        
        bool is_split(u32 offset, u32 len)
        {return offset+len > _buf_size;}
        
        void unite_enqueue(u32 offset, const char *data, u32 data_len);
        void unite_dequeue(u32 offset, char *buff, u32 data_len);
        
        void split_enqueue(u32 offset, const char *data, u32 data_len);
        void split_dequeue(u32 offset, char *buff, u32 data_len);
    };
    
    struct indented_loop_Q
    {
        int enqueue(const char *data, u32 data_len);
        int dequeue(char *buf, u32 buf_size, u32& data_len);

        indented_loop_Q() : _h(0), _loop_q(0){}
        loop_Q_head* _h;
        loop_Q* _loop_q;
        
        u32 size(){return _loop_q->_buf_size;}
        u32 head(){return _h->_head;}
        u32 tail(){return _h->_tail;}

        u32 used()
        {
            return
            (_h->_tail + _loop_q->_buf_size - (_h->_head+1))
            % _loop_q->_buf_size;
        }
        u32 free()
        {
            return
            (_h->_head + _loop_q->_buf_size - _h->_tail)
            % _loop_q->_buf_size;
        }
        //  used() + free() + 1 = size()
        
        void increase_tail(u32 len)
        {_h->_tail = (_h->_tail + len ) % _loop_q->_buf_size;}
        void increase_head(u32 len)
        {_h->_head = (_h->_head + len ) % _loop_q->_buf_size;}
        
        void init_attach(char* mem);    //  attach on existed queue
        void init_clear(u32 mem_size);  //  init a virgin mem to queue
        void fini_detach();
    };
    
    typedef indented_loop_Q mem_Q;
}

#endif
