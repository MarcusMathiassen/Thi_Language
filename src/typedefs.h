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

#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stddef.h>
#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef s8 bool;

#define ERROR_UNHANDLED_KIND(str) error("[%s:%s:%d] Unhandled case '%s'", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color(str));
#define UNREACHABLE error("[%s:%s:%d] %s", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color("UNREACHABLE"));
#define UNFINISHED error("[%s:%s:%d] %s", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color("UNFINISHED"));
#define LOCATION_OF_ast_to_str(module, node) strf("%s:%d:%d", module->Module.name, node->loc_info.line_pos, node->loc_info.col_pos)

#define true 1
#define false 0

#endif
