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

#include "utility.h"
#include "constants.h"
 // assert
#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h>  // printf, vprintf
#include <stdlib.h> // malloc, realloc, calloc
#include <string.h> // memcpy
#include <time.h>   // timeval
#include <unistd.h>

//------------------------------------------------------------------------------
//                               Printing Functions
//------------------------------------------------------------------------------

void _info(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RGB_GRAY);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
}

void warning(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", YELLOW);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
}

void success(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", GREEN);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
}

void error(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RED);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
    exit(1);
}

void _info_no_newline(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RGB_GRAY);
    vprintf(fmt, args);
    printf(RESET);
    va_end(args);
}

void warning_no_newline(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", YELLOW);
    vprintf(fmt, args);
    printf(RESET);
    va_end(args);
}

void success_no_newline(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", GREEN);
    vprintf(fmt, args);
    printf(RESET);
    va_end(args);
}

void error_no_newline(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RED);
    vprintf(fmt, args);
    printf(RESET);
    va_end(args);
    exit(1);
}
//------------------------------------------------------------------------------
//                               File Utility Functions
//------------------------------------------------------------------------------

#define DEBUG_PRINT_ENTRY info_no_newline("%s: %s", __func__, filename);
#define DEBUG_PRINT_NONE_FOUND info(" -> NONE FOUND.");
#define DEBUG_PRINT_EXIT info(" -> %s", str);

char* get_file_extension(char* filename) {
    xassert(filename);
    DEBUG_PRINT_ENTRY;
    s64 len = strlen(filename);
    s64 i = 0;
    // myfile.txt => txt
    // start from the back..
    // ./myfile.txt => txt ..
    s64 last_dot_pos = 0;
    // Find the position of the last dot from starting from the back
    while (len - i > 0) {
        if (filename[len - i] == '.') last_dot_pos = len - i;
        ++i;
    }
    if (last_dot_pos == 0) { // dot files are not seen as extensions
        DEBUG_PRINT_NONE_FOUND;
        return NULL; // we didnt find any
    }
    s64 start_of_ext = len - last_dot_pos;
    char* str = xmalloc(start_of_ext + 2);
    memcpy(str, filename + len - start_of_ext, start_of_ext);
    str[start_of_ext] = 0;
    DEBUG_PRINT_EXIT;
    return str;
}

char* remove_file_extension(char* filename) {
    xassert(filename);
    DEBUG_PRINT_ENTRY;
    s64 len = strlen(filename);
    s64 i = 0;
    while (i < len) {
        if (filename[len - (i)] == '.') break;
        ++i;
    }
    if (i == 0 || i == len) {
        DEBUG_PRINT_NONE_FOUND;
        return NULL; // we didnt find any
    }
    char* str = xmalloc(len - i + 1);
    memcpy(str, filename, len - i);
    str[len - i] = 0;
    DEBUG_PRINT_EXIT;
    return str;
}

char* get_file_directory(char* filename) {
    xassert(filename);
    DEBUG_PRINT_ENTRY;
    s64 len = strlen(filename);
    while (len > 0) {
        if (filename[len] == '/') break;
        --len;
    }
    if (len == 0) {
        DEBUG_PRINT_NONE_FOUND;
        return NULL; // we didnt find any
    }
    ++len; // we preserve the '/'
    char* str = xmalloc(len + 1);
    memcpy(str, filename, len);
    str[len] = 0;
    DEBUG_PRINT_EXIT;
    return str;
}

char* get_file_name(char* filename) {
    xassert(filename);
    DEBUG_PRINT_ENTRY;
    s64 len = strlen(filename);
    s64 i = 1;
    while (i < len) {
        if (filename[len - i] == '/') break;
        ++i;
    }
    if (i == 1) {
        DEBUG_PRINT_NONE_FOUND;
        return NULL; // we didnt find any
    }
    char* str = xmalloc(i + 1);
    memcpy(str, filename + len + 1 - i, i); // +1 skips the '/'
    str[i] = 0;
    DEBUG_PRINT_EXIT;
    return str;
}
void write_to_file(char* filename, char* buffer) {
    xassert(filename);
    xassert(buffer);
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Error opening file!\n");
        exit(1);
    }
    fputs(buffer, f);
    fclose(f);
}

char* get_file_content(char* filename) {
    xassert(filename);
    char* buffer = NULL;
    s64 string_size, read_size;
    FILE* f = fopen(filename, "r");
    if (!f) error("failed to open file: %s", filename);

    if (f) {
        // Seek the last byte of the file
        fseek(f, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(f);
        // go back to the start of the file
        rewind(f);

        // Allocate a string that can hold it all
        buffer = xmalloc(string_size + 1);

        // Read it all in one operation
        read_size = fread(buffer, sizeof(u8), string_size, f);

        // fread doesn't set it so put a \0 in the last position
        // and buffer is now officially a string
        buffer[string_size] = '\0';

        if (string_size != read_size) {
            // Something went wrong, throw away the memory and set
            // the buffer to NULL
            free(buffer);
            buffer = NULL;
        }

        // Always remember to close the file.
        fclose(f);
    }

    return buffer;
}

//------------------------------------------------------------------------------
//                               General Purpose
//------------------------------------------------------------------------------

void* _malloc(s64 bytes, char* file, char* func, s64 line) {
    xassert(bytes != 0);
    void* alloc = malloc(bytes);
    if (!alloc) error("[%s:%s:%lld] malloc(%lld) failed", file, func, line, bytes);
    // info("[%s:%s:%lld] malloc(%lld) called", file, func, line, bytes);
    return alloc;
}

void* _calloc(s64 size, s64 bytes, char* file, char* func, s64 line) {
    xassert(size != 0);
    xassert(bytes != 0);
    void* alloc = calloc(size, bytes);
    if (!alloc) error("[%s:%s:%lld] calloc(%lld, %lld) failed", file, func, line, size, bytes);
    // info("[%s:%s:%lld] calloc(%lld, %lld) called", file, func, line, size, bytes);
    return alloc;
}

void* _realloc(void* ptr, s64 bytes, char* file, char* func, s64 line) {
    xassert(ptr);
    xassert(bytes != 0);
    void* alloc = realloc(ptr, bytes);
    if (!alloc) error("[%s:%s:%lld] realloc(%lld, %llu) failed", file, func, line, (u64)ptr, bytes);
    // info("[%s:%s:%lld] realloc(%lld, %llu) called", file, func, line, (u64)ptr, bytes);
    return alloc;
}

char* strn(char* start, char* end) {
    xassert(start && end);
    s64 len = end - start;
    char* str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = 0;
    return str;
}


char* strf(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);

    char* str = xmalloc(n);

    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);

    return str;
}

char* get_indentation_as_str(u64 indent_level) {
    char* s = "";
    xassert(DEFAULT_INDENT_LEVEL == 4);
    while (indent_level > DEFAULT_INDENT_LEVEL) {
        s = strf("%s    ", s);
        indent_level -= DEFAULT_INDENT_LEVEL;
    }
    return s;
}

// Color Whell
char* colors[6] = {
    "\033[31m", // red
    "\033[32m", // green
    "\033[33m", // yellow
    "\033[34m", // blue
    "\033[35m", // magenta
    "\033[36m", // cyan,
};
int counter = 0;
int colors_count = 6;
char* get_previous_color(void) {
    if (counter == 0) counter = 6;
    return colors[--counter];
}
char* get_next_color(void) {
    if (counter == colors_count) counter = 0;
    return colors[counter++];
}
char* wrap_with_colored_parens(char* str) {
    xassert(str);
    char* current_color = get_next_color();
    return strf("%s(\033[00m%s%s)\033[00m", current_color, str, current_color);
}

char* ucolor(char* str) {
    return give_unique_color(str);
}

char* give_unique_color(char* str) {
    xassert(str);
    char* current_color = get_next_color();
    xassert(current_color);
    return strf("%s%s\033[00m", current_color, str);
}
char* get_unique_color(void) {
    return get_next_color();
}

void _tassert(char* expr_str, char* file, char* func, int line, char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);
    char* str = xmalloc(n);
    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);
    error("[%s:%s:%s] ASSERT %s %s", ucolor(file), ucolor(func), ucolor(strf("%d", line)), ucolor(expr_str), str);
}

//

//------------------------------------------------------------------------------
//                               Timing Utility Functions
//------------------------------------------------------------------------------

/*
author: jbenet
os x, compile with: gcc -o testo test.c
linux, compile with: gcc -o testo test.c -lrt
*/

#include <stdio.h>
#include <sys/time.h>
#include <time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

f64 get_time(void) {
    f64 ms;
    time_t s;
    struct timespec ts;

#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
    s = ts.tv_sec;
    ms = ts.tv_nsec / 1.0e6;
    if (ms > 999.0) {
        ++s;
        ms = 0.0;
    }
    return ms;
}

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------
void utility_tests(void) {
    // These leak memory.

    // get_file_directory
    xassert(strcmp(get_file_directory("./b/m.thi"), "./b/") == 0);
    xassert(get_file_directory("m.thi") == NULL);
    xassert(strcmp(get_file_directory("./b/m/m.thi"), "./b/m/") == 0);

    // get_file_extension
    xassert(strcmp(get_file_extension("./b/m.thi"), ".thi") == 0);
    xassert(strcmp(get_file_extension(".rcx.txt"), ".txt") == 0);
    xassert(get_file_extension("fefem") == NULL);
    xassert(strcmp(get_file_extension("./b/mthigegerg/o.hrifj"), ".hrifj") == 0);

    // remove_file_extension
    xassert(strcmp(remove_file_extension("./b/m.thi"), "./b/m") == 0);
    xassert(strcmp(remove_file_extension(".rcx.txt"), ".rcx") == 0);
    xassert(remove_file_extension("fefem") == NULL);
    xassert(strcmp(remove_file_extension("./b/mthigegerg/o.hrifj"), "./b/mthigegerg/o") == 0);

    // get_file_name
    xassert(strcmp(get_file_name("./b/m.thi"), "m.thi") == 0);
    xassert(strcmp(get_file_name("./b/m/hergergerg.thi"), "hergergerg.thi") == 0);
    xassert(get_file_name("./b/m/") == NULL);
}
