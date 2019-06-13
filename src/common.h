// Copyright (c) 2019 Marcus Mathiassen

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

#define COMPILER_VERSION "0.1"

#define TIMERS_SORT false
#define SHOW_TIMERS_WITH_SUFFIX true

#define RESET "\033[0m"
#define GRAY "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RGB_GRAY "\033[38;2;110;110;110;m"
#define RGB_WHITE "\033[38;2;255;255;255;m"

#define DEBUG_MEMORY_ALLOCATIONS 0

#define DEFAULT_FOR_IN_ITERATOR_NAME "it"
#define DEFAULT_INDENT_LEVEL 4

#define DEFAULT_POINTER_SIZE 8
#define DEFAULT_ENUM_SIZE 8

#define DEFAULT_INT_BYTE_SIZE 8
#define DEFAULT_FLOAT_BYTE_SIZE 8

#define DEFAULT_INT_TYPE_AS_STRING (DEFAULT_INT_BYTE_SIZE == 8) ? "s64" : "s32"
#define DEFAULT_BIG_INT_TYPE_AS_STRING "s64"
#define DEFAULT_FLOAT_TYPE_AS_STRING (DEFAULT_FLOAT_BYTE_SIZE == 8) ? "f64" : "f32"
#define DEFAULT_BIG_FLOAT_TYPE_AS_STRING "f64"

#define X64_ASM_MACOS_STACK_PADDING 16 // is 16 on macOS

#define DEFAULT_FUNCTION_ENTRY_LABEL_NAME ".begin"
#define DEFAULT_FUNCTION_END_LABEL_NAME ".end"

#define AST_REF_LIST_STARTING_ALLOC 16
#define TYPE_REF_LIST_STARTING_ALLOC 16
#define INTERN_ARRAY_STARTING_ALLOC 100
#define TOKEN_ARRAY_STARTING_ALLOC 100

#define DEFAULT_MAP_STARTING_TABLE_SIZE 24

#define STRING_STARTING_ALLOC 24
#define foreach(i, c) for (s64 (i) = 0; (i) < (c); ++(i))

typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;
typedef s8       bool;

#define bytes(n) (n)
#define kilobytes(n) ((u64)bytes((u64)n) * 1024ULL)
#define megabytes(n) ((u64)kilobytes((u64)n) * 1024ULL)
#define gigabytes(n) ((u64)megabytes((u64)n) * 1024ULL)
#define terabytes(n) ((u64)gigabytes((u64)n) * 1024ULL)

#define to_nanoseconds(n) (n)
#define to_microseconds(n) ((u64)to_nanoseconds(n) / 1000ULL)
#define to_milliseconds(n) ((u64)to_microseconds(n) / 1000ULL)
#define to_seconds(n) ((u64)to_milliseconds(n) / 1000ULL)
#define to_minutes(n) ((u64)to_seconds(n) / 60ULL)
#define to_hours(n) ((u64)to_minutes(n) / 60ULL)

#define nanoseconds(n) (n)
#define microseconds(n) ((u64)nanoseconds(n) * 1000ULL)
#define milliseconds(n) ((u64)microseconds(n) * 1000ULL)
#define seconds(n) ((u64)milliseconds(n) * 1000ULL)
#define minutes(n) ((u64)seconds(n) * 60ULL)
#define hours(n) ((u64)minutes(n) * 60ULL)

#define TASSERT_KIND_IN_RANGE(ENUM, kind) tassert(0 <= kind && kind < _##ENUM##_COUNT_, "kind = %d", kind)
#define ERROR_UNHANDLED_KIND(str) default: error("[%s:%s:%d] Unhandled case '%s'", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color(str));
#define UNREACHABLE error("[%s:%s:%d] %s", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color("UNREACHABLE"));
#define UNFINISHED error("[%s:%s:%d] %s", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color("UNFINISHED"));
#define LOCATION_OF(module, node) strf("%s:%d:%d", module->Module.name, node->loc_info.line, node->loc_info.col)

#define true 1
#define false 0

// ------------------------ OPTIONS ---------------------------
#define DEFAULT_GIGABYTE_SUFFIX "gb"
#define DEFAULT_MEGABYTE_SUFFIX "mb"
#define DEFAULT_KILOBYTE_SUFFIX "kb"
#define DEFAULT_BYTE_SUFFIX "b"

#define DEFAULT_HOURS_SUFFIX "h"
#define DEFAULT_MINUTES_SUFFIX "m"
#define DEFAULT_SECONDS_SUFFIX "s"
#define DEFAULT_MILLISECONDS_SUFFIX "ms"
#define DEFAULT_MICROSECONDS_SUFFIX "us"
#define DEFAULT_NANOSECONDS_SUFFIX "ns"















#endif
