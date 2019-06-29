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

//@Audit: includes

#include "utility.h"
#include "list.h" // List
#include "stack.h" // Stack
#include "string.h" // string

#include <stdio.h>  // printf, vprintf
#include <stdlib.h> // malloc, realloc, calloc
#include <string.h> // memcpy, strlen

#include <unistd.h>
#include <ctype.h>       // NOTE(marcus): what do i use this
#include <sys/ioctl.h>    // NOTE(marcus): what do i use this for?
//------------------------------------------------------------------------------
//                               Printing Functions
//------------------------------------------------------------------------------

void _debug(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RGB_GRAY);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
}

void info(char* fmt, ...) {
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

void _debug_no_newline(char* fmt, ...) {
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

#define DEBUG_PRINT_ENTRY debug_no_newline("%s: %s", __func__, filename);
#define DEBUG_PRINT_NONE_FOUND debug(" -> NONE FOUND.");
#define DEBUG_PRINT_EXIT debug(" -> %s", str);

char* get_file_extension(char* filename) {
    xassert(filename);
    DEBUG_PRINT_ENTRY;
    s64 len = xstrlen(filename);
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
    s64 len = xstrlen(filename);
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
    s64 len = xstrlen(filename);
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
    s64 len = xstrlen(filename);
    s64 i = 0;
    while (i < len) {
        if (filename[len-i] == '/') break;
        ++i;
    }
    if (i == len) {
        return filename;
    } else if (i == 1) {
        return NULL;
    }
    char* str = xmalloc(i + 1);
    memcpy(str, filename + (len-i+1), i); // +1 skips the '/'
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
    debug("%s: %s", (char*)__func__, filename);
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
    #if DEBUG_MEMORY_ALLOCATIONS
    debug("[%s:%s:%lld] malloc(%lld) called", file, func, line, bytes);
    #endif
    return alloc;
}

void* _calloc(s64 size, s64 bytes, char* file, char* func, s64 line) {
    xassert(size != 0);
    xassert(bytes != 0);
    void* alloc = calloc(size, bytes);
    if (!alloc) error("[%s:%s:%lld] calloc(%lld, %lld) failed", file, func, line, size, bytes);
    #if DEBUG_MEMORY_ALLOCATIONS
    // debug("[%s:%s:%lld] calloc(%lld, %lld) called", file, func, line, size, bytes);
    #endif
    return alloc;
}

void* _realloc(void* ptr, s64 bytes, char* file, char* func, s64 line) {
    xassert(ptr);
    xassert(bytes != 0);
    void* alloc = realloc(ptr, bytes);
    if (!alloc) error("[%s:%s:%lld] realloc(%lld, %llu) failed", file, func, line, (u64)ptr, bytes);
    #if DEBUG_MEMORY_ALLOCATIONS
    // debug("[%s:%s:%lld] realloc(%lld, %llu) called", file, func, line, (u64)ptr, bytes);
    #endif
    return alloc;
}

u64 _strlen(char* str, char* file, char* func, s64 line) {
    xassert(str);
    u64 len = strlen(str);
    // if (!len) error("[%s:%s:%lld] strlen(%s) failed", file, func, line, str);
    #if DEBUG_MEMORY_ALLOCATIONS
    // debug("[%s:%s:%lld] strlen(%s) called", file, func, line, str);
    #endif
    return len;   
}

char* get_colored_minimap_of_file(char* file, char c) {
    char* s = get_file_content(file);
    u64 i = 0;
    List* lines = make_list();
    char* line_start = s;
    char* line_end = NULL;
    while (s[i] != '\0') {
        if (!isspace(s[i])) s[i] = c;
        if (s[i] == '\n') { 
            line_end = &s[i];
            list_append(lines, strn(line_start, line_end));
            line_start = line_end;
        }
        ++i;
    }
    string* k = make_string("");
    list_foreach(lines) {
        char* line = it->data;
        string_append(k, ucolor(line));
    }
    return string_data(k);
}

char* time_with_suffix(u64 n) {
    u64 m = (n >= hours(1ULL) ? 
            n / hours(1ULL) : n >= minutes(1ULL) 
            ? n / minutes(1ULL) :  n >= seconds(1ULL) 
            ? n / seconds(1ULL) :  n >= milliseconds(1ULL) 
            ? n / milliseconds(1ULL) : n >= microseconds(1ULL) 
            ? n / microseconds(1ULL) :  n >= nanoseconds(1ULL) 
            ? n / nanoseconds(1ULL) : n);

    char* suffix = (n >= hours(1ULL) ? DEFAULT_HOURS_SUFFIX
        : n >= minutes(1ULL) ? DEFAULT_MINUTES_SUFFIX
        : n >= seconds(1ULL) ? DEFAULT_SECONDS_SUFFIX
        : n >= milliseconds(1ULL) ? DEFAULT_MILLISECONDS_SUFFIX
        : n >= microseconds(1ULL) ? DEFAULT_MICROSECONDS_SUFFIX
        : n >= nanoseconds(1ULL) ? DEFAULT_NANOSECONDS_SUFFIX
        : "");
    
    return strf("%llu%s", m, suffix);
}

char* size_with_suffix(u64 n) {
    u64 m = (n >= gigabytes(1ULL) ? n / gigabytes(1ULL) : n >= megabytes(1ULL) ? n / megabytes(1ULL) :  n >= kilobytes(1ULL) ? n / kilobytes(1ULL) :  n >= bytes(1ULL) ? n / bytes(1ULL) : n);
    char* suffix = (n >= gigabytes(1ULL) ? DEFAULT_GIGABYTE_SUFFIX
        : n >= megabytes(1ULL) ? DEFAULT_MEGABYTE_SUFFIX
        :  n >= kilobytes(1ULL) ? DEFAULT_KILOBYTE_SUFFIX
        :  n >= bytes(1ULL) ? DEFAULT_BYTE_SUFFIX
        : "");
    return strf("%llu%s", m, suffix);
}

typedef struct {
    s8 col;
    s8 row;
    List* col_desc;
    List* row_desc;
} Table;

char* table_entry(char* left, char* right) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    s64 len = xstrlen(left);
    s64 ms_l = xstrlen(right);
    s64 padding = w.ws_col - len - ms_l;
    return strf("%s%*s%s", left, padding, "", right);
}

char* str_replace_center(char* str, char* into) {
    s64 slen = xstrlen(str);
    s64 ilen = xstrlen(into);

    s64 imid = ilen/2;
    s64 i_starting_point = imid - slen/2;

    for (s64 i = i_starting_point; i < slen+i_starting_point; ++i) {
        into[i] = str[i-i_starting_point];
    }

    return into;
}

char* pad_out_full_width(char ch) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    char* str = xmalloc(w.ws_col + 1);
    foreach(i, w.ws_col)
        str[i] = ch;
    str[w.ws_col] = '\0';
    return str;
}
char* align_center(char* str) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    s64 len = xstrlen(str);
    s64 middle = w.ws_col / 2;
    s64 padding = middle - len / 2;

    return strf("%*s%s", padding, "", str);    
}

char* strn(char* start, char* end) {
    tassert(start && end && (start <= end), "%llu -> %llu", start, end);
    s64 len = end - start;
    char* str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = '\0';
    return str;
}

char* strf(char* fmt, ...) {
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);
    char* str = xmalloc(n + 1);
    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);
    str[n] = '\0';
    return str;
}

char* vstrf(char* fmt, va_list args) {
    xassert(fmt);
    va_list args_count;
    va_copy(args_count, args);
    s64 n = 1 + vsnprintf(0, 0, fmt, args_count);
    va_end(args_count);
    char* str = xmalloc(n + 1);
    vsnprintf(str, n, fmt, args);
    str[n] = '\0';
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
static char* colors[6] = {
    "\033[31m", // red
    "\033[32m", // green
    "\033[33m", // yellow
    "\033[34m", // blue
    "\033[35m", // magenta
    "\033[36m", // cyan,
};
static u64 counter = 0;
static u64 colors_count = 6;
char* get_previous_color(void) {
    if (counter == 0) counter = 6;
    return colors[--counter];
}
char* get_next_color(void) {
    return colors[counter++ % colors_count];
}
char* wrap_with_colored_parens(char* str) {
    xassert(str);
    char* current_color = get_next_color();
    return strf("%s(\033[00m%s%s)\033[00m", current_color, str, current_color);
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

#ifdef __APPLE__
#include <mach/mach_time.h>
#elif __unix__
#include <sys/time.h>
#include <time.h>
#endif
u64 get_time(void) {
    #ifdef __APPLE__
        mach_timebase_info_data_t info;
        if (mach_timebase_info(&info) != KERN_SUCCESS)
            abort();
        return (mach_absolute_time() * info.numer / info.denom);
    #elif __unix__
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return ((uint64_t) tv.tv_sec * (uint64_t) 1000000 + (uint64_t) tv.tv_usec) * 1000;
    #endif
}

List* timer_list;
static Stack* timer_stack;
static u64 timer_indent;
#define timer_indent_amount 4
Timer* push_timer(char* desc) {
    xassert(desc);
    Timer* tm = xmalloc(sizeof(Timer));
    tm->ns = get_time();
    #if !TIMERS_SORT
    tm->desc = strf("%*s%s", timer_indent, "", desc);
    timer_indent += timer_indent_amount;
    #else 
    tm->desc = desc;
    #endif
    stack_push(timer_stack, tm);
    return tm;
}

Timer* pop_timer(void) {
    Timer* tm = stack_pop(timer_stack);
    tm->ns = get_time() - tm->ns;
    list_append(timer_list, tm);
    #if !TIMERS_SORT
    timer_indent -= timer_indent_amount;
    #endif
    return tm;
}

void initilize_timers(void) {
    timer_indent = 0;
    timer_list = make_list();
    timer_stack = make_stack();
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
    xassert(strcmp(get_file_extension("test.thi"), ".thi") == 0);
    xassert(get_file_extension("fefem") == NULL);
    xassert(strcmp(get_file_extension("./b/mthigegerg/o.hrifj"), ".hrifj") == 0);

    // remove_file_extension
    xassert(strcmp(remove_file_extension("./b/m.thi"), "./b/m") == 0);
    xassert(strcmp(remove_file_extension(".rcx.txt"), ".rcx") == 0);
    xassert(remove_file_extension("fefem") == NULL);
    xassert(strcmp(remove_file_extension("./b/mthigegerg/o.hrifj"), "./b/mthigegerg/o") == 0);

    // get_file_name
    xassert(strcmp(get_file_name("./b/m.thi"), "m.thi") == 0);
    xassert(strcmp(get_file_name("test.thi"), "test.thi") == 0);
    xassert(strcmp(get_file_name("./b/m/hergergerg.thi"), "hergergerg.thi") == 0);
    xassert(get_file_name("./b/m/") == NULL);
}
