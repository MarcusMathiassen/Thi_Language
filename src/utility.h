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
//------------------------------------------------------------------------------
//                               Printing Functions
//------------------------------------------------------------------------------
void info(char* fmt, ...);
void warning(char* fmt, ...);
void success(char* fmt, ...);
void error(char* fmt, ...);

void info_no_newline(char* fmt, ...);
void warning_no_newline(char* fmt, ...);
void success_no_newline(char* fmt, ...);
void error_no_newline(char* fmt, ...);
//------------------------------------------------------------------------------
//                               File Functions
//------------------------------------------------------------------------------
char* get_file_path_from_directory(char* dir, char* filename);
char* get_file_extension(char* filename);
char* remove_file_extension(char* filename);
char* get_file_directory(char* filename);
char* get_file_name(char* filename);
char* get_file_content(char* filename);
void  write_to_file(char* filename, char* buffer);
//------------------------------------------------------------------------------
//                               General Purpose
//------------------------------------------------------------------------------
void* _malloc(s64 bytes, char* file, int line);
void* _calloc(s64 size, s64 bytes, char* file, int line);
void* _realloc(void* ptr, s64 bytes, char* file, int line);
#define xmalloc(n) _malloc(n, __FILE__, __LINE__)
#define xrealloc(n, m) _realloc(n, m, __FILE__, __LINE__)
#define xcalloc(n, m) _calloc(n, m, __FILE__, __LINE__)
char* strf(char* fmt, ...);
char* get_indentation_as_str(u64 indent_level);
char* get_previous_color(void);
char* get_next_color(void);
char* wrap_with_colored_parens(char* str);
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
