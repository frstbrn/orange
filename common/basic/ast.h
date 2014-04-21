//
//  ast.h
//  orange
//
//  Created by f on 14-4-12.
//  Copyright (c) 2014年 aurora. All rights reserved.
//

#ifndef orange_ast_h
#define orange_ast_h

#include <cassert>
#include <cstdio>

#ifndef fatal_log
#   define fatal_log printf
#endif // fatal_log

#ifndef get_stack_trace
#   define get_stack_trace() "undefined trace info"
#endif // get_stack_trace

// do-while(0) was not used here
#define _massert_with_log(sentence, next)\
{if (!(sentence)){\
fatal_log("Assertion: (%s), file: %s, line: %d, function: %s"\
, #sentence, __FILE__, __LINE__, __FUNCTION__);\
fatal_log("Assertion Stack Trace: \n%s\n"\
, get_stack_trace());\
assert((sentence) && #sentence);\
next;\
}}

#define ast(sentence) _massert_with_log(sentence, )
#define ast_ret(sentence, x) _massert_with_log(sentence, return x)
#define ast_ret_void(sentence) _massert_with_log(sentence, return )
#define ast_cont(sentence) _massert_with_log(sentence, continue)
#define ast_break(sentence) _massert_with_log(sentence, break)

#define ast_static(sentence) do{\
extern int __sa##__LINE__[sentence ? 0 : -1];\
(void)__sa##__LINE__[sentence ? 0 : -1];\
}while(0)

#endif
