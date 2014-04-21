//
//  mem_q.cpp
//  orange
//
//  Created by f on 14-4-11.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#include <cerrno>
#include <cstring>

#include "common/basic/ast.h"

#include "mem_q.h"

using namespace matrix;

void indented_loop_Q::init_attach(char* mem)
{
    ast(_h == 0 && _loop_q == 0);
    ast(mem);
    
    _h = (loop_Q_head*)mem;
    _loop_q = (loop_Q*)mem + sizeof(loop_Q_head);
}

void indented_loop_Q::fini_detach()
{
    ast(_h && _loop_q);
    _h = 0;
    _loop_q = 0;
}

void indented_loop_Q::init_clear(u32 mem_size)
{
    ast(_h && _loop_q);
    ast(mem_size > sizeof(loop_Q_head) + sizeof(loop_Q));

    _h->_head     = 0;
    _h->_tail     = 1;

    //  header 1 byte in front of data forever, but total-size must be honest
    _loop_q->_buf_size
    = mem_size
    - (u32)
    (_loop_q->_buf - (char*)_h);
}

////////////////////////////////////////////////////////////////////////////////

typedef u32 Q_pkg_head;
static const u32 C_PKG_HEAD_SIZE = sizeof(Q_pkg_head);

int indented_loop_Q::enqueue(const char *data, u32 data_len)
{
    //  first, if enough space
    if (data_len + C_PKG_HEAD_SIZE > free())
    {
        return -ENOMEM;
    }
    
    // second, enqueue pkg header
    _loop_q->enqueue(tail(), (const char*)&data_len, C_PKG_HEAD_SIZE);
    
    // third, enqueue pkg
    _loop_q->enqueue((tail() + C_PKG_HEAD_SIZE) % size()
                     , data, data_len);
    
    //  fourth, increase tail
    increase_tail(data_len + C_PKG_HEAD_SIZE);
    
    return 0;
}

int indented_loop_Q::dequeue(char *buf, u32 buf_size, u32& data_len)
{
    //  first, any data available
    if (used() == 0)
    {
        return -EAGAIN;
    }
    
    ast(used() >= C_PKG_HEAD_SIZE);
    
    //  second, get pkg head, pkg size
    //  +1 here, visiable for user, not inside head() call
    _loop_q->dequeue((head()+1) % size(), (char *)&data_len, C_PKG_HEAD_SIZE);
    
    if (data_len > buf_size)
    {
        return -ENOMEM;
    }
    
    //  third, get data
    _loop_q->dequeue((head()+1 + C_PKG_HEAD_SIZE) % size()
                     , buf, data_len);
    
    //  fourth, tail ++
    increase_head(data_len + C_PKG_HEAD_SIZE);
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void loop_Q::unite_enqueue(u32 offset, const char* data, u32 data_len)
{
    offset %= _buf_size;
    memcpy(_buf + offset, data, data_len);
}

void loop_Q::unite_dequeue(u32 offset, char* buf, u32 data_len)
{
    offset = offset % _buf_size;
    memcpy(buf, _buf + offset, data_len);
}

void loop_Q::split_enqueue(u32 offset, const char *data, u32 data_len)
{
    offset %= _buf_size;
    u32 front_len = _buf_size - offset;
    memcpy(_buf + offset, data, front_len);
    
    u32 rear_len = data_len - front_len;    //  == offset + data_len - _buf_size
    memcpy(_buf, data + front_len, rear_len);
}

void loop_Q::split_dequeue(u32 offset, char* buf, u32 data_len)
{
    offset %= _buf_size;
    u32 front_len = _buf_size - offset;
    memcpy(buf, _buf + offset, front_len);
    
    u32 rear_len = data_len - front_len;
    memcpy(buf + front_len, _buf, rear_len);
}


