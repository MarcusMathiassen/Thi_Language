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

#ifndef UTILITY_H
#define UTILITY_H
#include "typedefs.h"
#include <stdlib.h> // malloc, calloc, realloc
//------------------------------------------------------------------------------
//                               Printing Functions
//------------------------------------------------------------------------------
// These must always be called with a fmt.
// ex. 'info(my_str)' will crash if 'my_str' contains format specifiers.
//      so always do this 'info("%s", my_str)' instead.

#ifdef NDEBUG
#define info(fmt, ...)
#define info_no_newline(fmt, ...)
#else
#define info(fmt, ...) (_info(fmt, ##__VA_ARGS__))
#define info_no_newline(fmt, ...) (_info_no_newline(fmt, ##__VA_ARGS__))
#endif

void _info(char* fmt, ...);
void warning(char* fmt, ...);
void success(char* fmt, ...);
void error(char* fmt, ...);

void _info_no_newline(char* fmt, ...);
void warning_no_newline(char* fmt, ...);
void success_no_newline(char* fmt, ...);
void error_no_newline(char* fmt, ...);
//------------------------------------------------------------------------------
//                               File Functions
//------------------------------------------------------------------------------
char* get_file_extension(char* filename);
char* remove_file_extension(char* filename);
char* get_file_directory(char* filename);
char* get_file_name(char* filename);
char* get_file_content(char* filename);
void write_to_file(char* filename, char* buffer);
//------------------------------------------------------------------------------
//                               General Purpose
//------------------------------------------------------------------------------

#ifdef NDEBUG
#define xmalloc(bytes) malloc(bytes)
#define xcalloc(size, bytes) calloc(size, bytes)
#define xrealloc(ptr, bytes) realloc(ptr, bytes)
#else
#define xmalloc(bytes) (_malloc(bytes, __FILE__, (char*)__func__, __LINE__))
#define xcalloc(size, bytes) (_calloc(size, bytes, __FILE__, (char*)__func__, __LINE__))
#define xrealloc(ptr, bytes) (_realloc(ptr, bytes, __FILE__, (char*)__func__, __LINE__))
#endif
void* _malloc(s64 bytes, char* file, char* func, s64 line);
void* _calloc(s64 size, s64 bytes, char* file, char* func, s64 line);
void* _realloc(void* ptr, s64 bytes, char* file, char* func, s64 line);

char* strf(char* fmt, ...);
char* get_indentation_as_str(u64 indent_level);
typedef struct {
    s64 line_pos;
    s64 col_pos;
} Loc_Info;

#ifdef NDEBUG
#define tassert(EX, FMT, ...)
#else
#define tassert(EX, FMT, ...) (void)((EX) || (_tassert(#EX, __FILE__, (char*)__func__, __LINE__, FMT, ##__VA_ARGS__), 0))
#endif
void _tassert(char* expr_str, char* file, char* func, int line, char* fmt, ...);

//------------------------------------------------------------------------------
//                               Colors
//------------------------------------------------------------------------------
char* get_previous_color(void);
char* get_next_color(void);
char* wrap_with_colored_parens(char* str);
char* ucolor(char* str);
char* give_unique_color(char* str);
char* get_unique_color(void);
//------------------------------------------------------------------------------
//                               Timing Functions
//------------------------------------------------------------------------------
f64 get_time(void);
//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------
void utility_tests(void);
#endif
