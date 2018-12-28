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

//------------------------------------------------------------------------------
//                               File Functions
//------------------------------------------------------------------------------
char* get_file_path_from_directory(char* dir, char* filename);
char* get_file_extension(char* filename);
char* get_file_directory(char* filename);
char* get_file_name(char* filename);
char* get_file_content(char* filename);
void write_to_file(char* filename, char* buffer);

//------------------------------------------------------------------------------
//                               General Purpose
//------------------------------------------------------------------------------
void* xmalloc(u64 bytes);
void* xcalloc(u64 size, u64 bytes);
void* xrealloc(void* ptr, u64 bytes);
u64 xstrlen(char* str);

char* strf(char* fmt, ...);

char* get_next_color(void);
char* wrap_with_colored_parens(char* str);
char* give_unique_color(char* str);
char* get_unique_color(void);

//------------------------------------------------------------------------------
//                               Timing Functions
//------------------------------------------------------------------------------

typedef struct
{
    f64 ms;
    char* desc;
} Timer;
f64 get_time(void);

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------

void utility_tests(void);

#endif
