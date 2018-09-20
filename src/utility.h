#ifndef UTILITY_H
#define UTILITY_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                               Printing Functions
//------------------------------------------------------------------------------

void info(const char* fmt, ...);
void warning(const char* fmt, ...);
void success(const char* fmt, ...);
void error(const char* fmt, ...);

//------------------------------------------------------------------------------
//                               File Functions
//------------------------------------------------------------------------------

char* get_file_path_from_dir(const char* dir, const char* filename);
char* get_file_ext(const char* filename);
char* get_file_dir(const char* filename);
char* get_file_name(const char* filename);
char* get_file_content(const char* filename);
void write_to_file(const char* filename, const char* buffer);

//------------------------------------------------------------------------------
//                               General Purpose
//------------------------------------------------------------------------------
void* xmalloc(u64 bytes);
void* xcalloc(u64 size, u64 bytes);
void* xrealloc(void* ptr, u64 bytes);
u64 xstrlen(const char* str);

char* strf(const char* fmt, ...);

const char* get_next_color(void);
char* wrap_with_colored_parens(char* str);
char* give_unique_color(char* str);
const char* get_unique_color(void);

//------------------------------------------------------------------------------
//                               Timing Functions
//------------------------------------------------------------------------------

typedef struct {
    f64 ms;
    const char* desc;
} Timer;
f64 get_time(void);
void push_timer(const char* desc);
void pop_timer(void);
Timer* get_timers(void);

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void utility_tests(void);

#endif
