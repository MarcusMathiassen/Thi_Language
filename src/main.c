// Copyright (c) 2020 Marcus Mathiassen

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>       // NOTE(marcus): what do i use this
#include <sys/ioctl.h>    // NOTE(marcus): what do i use this for?

#define RESET     "\033[0m"
#define GRAY      "\033[30m"
#define RED       "\033[31m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define RGB_GRAY  "\033[38;2;110;110;110;m"
#define RGB_WHITE "\033[38;2;255;255;255;m"

#define foreach(i, c)         for (s64 (i) = 0;   (i) < (c); ++(i))
#define foreach_reverse(i, c) for (s64 (i) = c-1; (i) >= 0;  --(i))

#define INTERNAL        static
#define GLOBAL_VARIABLE static
#define LOCAL_PERSISTS  static

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

typedef s8       b8;
typedef s16      b16;
typedef s32      b32;
typedef s64      b64;

#ifndef bool
#define bool u8
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#define CONST_ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))

#define BYTES(b) (b)
#define KILOBYTES(b) (BYTES(b) * 1024ULL)
#define MEGABYTES(b) (KILOBYTES(b) * 1024ULL)
#define GIGABYTES(b) (MEGABYTES(b) * 1024ULL)
#define TERABYTES(b) (GIGABYTES(b) * 1024ULL)

#define TO_NANOSECONDS(n)  (n)
#define TO_MICROSECONDS(n) (TO_NANOSECONDS(n) / 1000ULL)
#define TO_MILLISECONDS(n) (TO_MICROSECONDS(n) / 1000ULL)
#define TO_SECONDS(n)      (TO_MILLISECONDS(n) / 1000ULL)
#define TO_MINUTES(n)      (TO_SECONDS(n) / 60ULL)
#define TO_HOURS(n)        (TO_MINUTES(n) / 60ULL)

#define NANOSECONDS(n)  (n)
#define MICROSECONDS(n) (NANOSECONDS(n) * 1000ULL)
#define MILLISECONDS(n) (MICROSECONDS(n) * 1000ULL)
#define SECONDS(n)      (MILLISECONDS(n) * 1000ULL)
#define MINUTES(n)      (SECONDS(n) * 60ULL)
#define HOURS(n)        (MINUTES(n) * 60ULL)

#define GIGABYTE_SUFFIX "gb"
#define MEGABYTE_SUFFIX "mb"
#define KILOBYTE_SUFFIX "kb"
#define BYTE_SUFFIX     "b"

#define HOURS_SUFFIX        "h"
#define MINUTES_SUFFIX      "m"
#define SECONDS_SUFFIX      "s"
#define MILLISECONDS_SUFFIX "ms"
#define MICROSECONDS_SUFFIX "us"
#define NANOSECONDS_SUFFIX  "ns"

#if DEV
    #define ASSERT_KIND_IN_RANGE(ENUM, kind) tassert(0 <= kind && kind < _##ENUM##_count_, "kind = %d", kind)
    #define ERROR_UNHANDLED_KIND(str) default: error("[%s:%s:%d] Unhandled case '%s'", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color(str));
    #define UNREACHABLE error("[%s:%s:%d] %s", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color("UNREACHABLE"));
    #define UNFINISHED error("[%s:%s:%d] %s", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), __LINE__, give_unique_color("UNFINISHED"));
#else
    #define ASSERT_KIND_IN_RANGE(ENUM, kind)
    #define ERROR_UNHANDLED_KIND(str)
    #define UNREACHABLE
    #define UNFINISHED
#endif

#define FALLTHROUGH

#include "utility.c"
#include "list.c"
#include "stack.c"
#include "map.c"
#include "string.c"

typedef struct
{
    char* desc;
    u64 start;
    u64 end;
} Timer;

GLOBAL_VARIABLE List* timers;
GLOBAL_VARIABLE Stack* timers_stack;

INTERNAL void initilize_timers()
{
    timers = make_list();
    timers_stack = make_stack();
}

INTERNAL Timer* push_timer(char* desc)
{
    Timer* t = xmalloc(sizeof(Timer));
    t->desc = desc;
    t->start = get_time();
    stack_push(timers_stack, t);
    return t;
}

INTERNAL Timer* pop_timer()
{
    Timer* t = stack_pop(timers_stack);
    t->end = get_time();
    list_append(timers, t);
    return t;
}

INTERNAL u64 timer_elapsed_time(Timer* t)
{
    return t->end - t->start;
}

INTERNAL void assemble(char* asm_file, char* exec_name)
{
    string* comp_call = make_string_f("nasm -f macho64 %s.s -o %s.o", asm_file, exec_name);
    debug("Assembling with options '%s'", ucolor(string_data(comp_call)));
    system(string_data(comp_call));
}

INTERNAL void linking_stage(List* links, char* exec_name)
{
    string* link_call = make_string_f("ld -lc -macosx_version_min 10.15 -no_pie -o %s %s.o -e _main", exec_name, exec_name);
    debug("Linking with options '%s'", ucolor(string_data(link_call)));
    system(string_data(link_call));
    // Cleanup object files
}

#include "lex.c"
#include "parse.c"
#include "codegen.c"

int main(int argc, char** argv)
{
    initilize_timers();

    {
        push_timer("Total");

        char* source_file = argv[1];

        char* ext = get_file_extension(source_file);
        char* dir = get_file_directory(source_file);
        char* name = get_file_name(source_file);
        char* exec_name = remove_file_extension(name);

        push_timer("parse");
        AST* ast = parse(source_file);
        pop_timer();
        push_timer("codegen");
        char* code = to_x64(ast);
        pop_timer();

        char* name_with_ext_removed = remove_file_extension(name);
        write_to_file(strf("%s.s", name_with_ext_removed), code);
        
        push_timer("assemble");
        assemble(name_with_ext_removed, exec_name);
        pop_timer();
        push_timer("linking");
        linking_stage(make_list(), exec_name);
        pop_timer();

        pop_timer();
    }

    // system("clear");
    success(str_replace_center(" Thi ", pad_out_full_width('_')));
    list_foreach_reverse(timers)
    {
        Timer* t = it->data;
        success(table_entry(t->desc, time_with_suffix(timer_elapsed_time(t))));
    }

    return 0;
}
