#include "utility.h"

#include "stretchy_buffer.h" // sb_free
#include <assert.h>          // assert
#include <stdarg.h>          // va_list, va_start, va_end
#include <stdio.h>           // printf, vprintf
#include <stdlib.h>          // malloc
#include <string.h>          // memcpy
#include <time.h>            // timeval

//------------------------------------------------------------------------------
//                               Printing Functions
//------------------------------------------------------------------------------


#define RESET "\033[0m"
#define GRAY "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RGB_GRAY "\033[38;2;110;110;110;m"

void info(const char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    printf(RGB_GRAY);
    vprintf(fmt, args);
    printf(RESET "\n");
    va_end(args);
}

void warning(const char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    printf(YELLOW);
    vprintf(fmt, args);
    printf(RESET "\n");
    va_end(args);
}

void success(const char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    printf(GREEN);
    vprintf(fmt, args);
    printf(RESET "\n");
    va_end(args);
}

void error(const char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    printf(RED);
    vprintf(fmt, args);
    printf(RESET "\n");
    va_end(args);
    exit(1);
}

//------------------------------------------------------------------------------
//                               File Utility Functions
//------------------------------------------------------------------------------

char* get_file_path_from_dir(const char* dir, const char* filename) {
    assert(dir);
    assert(filename);
    char strbuf[1000];
    u64 d_len = strlen(dir);
    u64 f_len = strlen(filename);
    u64 len = d_len + f_len;
    assert(len < 1000);
    memcpy(strbuf, dir, d_len);              // copy dir into strbuf
    memcpy(strbuf + d_len, filename, f_len); // append filename
    char* str = xmalloc(len + 1);
    memcpy(str, strbuf, len);
    str[len] = 0;
    return str;
}

char* get_file_ext(const char* filename) {
    assert(filename);
    u64 len = strlen(filename);
    u64 i = 0;
    while (filename[len - (++i)] != '.')
        continue;
    ++len; // skip the '.'
    char* str = xmalloc(i + 1);
    memcpy(str, filename + len - i, i);
    str[i] = 0;
    return str;
}

char* get_file_dir(const char* filename) {
    assert(filename);
    u64 len = strlen(filename);
    while (filename[--len] != '/')
        continue;
    ++len; // we preserve the '/'
    char* str = xmalloc(len + 1);
    memcpy(str, filename, len);
    str[len] = 0;
    return str;
}
char* get_file_name(const char* filename) {
    assert(filename);
    u64 len = strlen(filename);
    u64 i = 0;
    while (filename[len - (++i)] != '/')
        continue;
    ++len; // skip the '/'
    char* str = xmalloc(i + 1);
    memcpy(str, filename + len - i, i);
    str[i] = 0;
    return str;
}
void write_to_file(const char* filename, const char* buffer) {
    assert(filename);
    assert(buffer);
    FILE* f = fopen(filename, "w");
    if (!f) {
        printf("Error opening file!\n");
        exit(1);
    }
    fputs(buffer, f);
    fclose(f);
}

char* get_file_content(const char* filename) {
    assert(filename);
    char* buffer = NULL;
    u64 string_size, read_size;
    FILE* handler = fopen(filename, "r");

    if (handler) {
        // Seek the last byte of the file
        fseek(handler, 0, SEEK_END);
        // Offset from the first to the last byte, or in other words, filesize
        string_size = ftell(handler);
        // go back to the start of the file
        rewind(handler);

        // Allocate a string that can hold it all
        buffer = (char*)malloc(sizeof(char) * (string_size + 1));

        // Read it all in one operation
        read_size = fread(buffer, sizeof(char), string_size, handler);

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
        fclose(handler);
    }

    return buffer;
}

//------------------------------------------------------------------------------
//                               General Purpose
//------------------------------------------------------------------------------

void* xmalloc(u64 bytes) {
    assert(bytes != 0);
    void* alloc = malloc(bytes);
    if (!alloc)
        error("alloc failed");
    return alloc;
}

void* xcalloc(u64 size, u64 bytes) {
    assert(size != 0);
    assert(bytes != 0);
    void* alloc = calloc(size, bytes);
    if (!alloc)
        error("calloc failed");
    return alloc;
}

void* xrealloc(void* ptr, u64 bytes) {
    assert(ptr);
    assert(bytes != 0);
    void* alloc = realloc(ptr, bytes);
    if (!alloc)
        error("realloc failed");
    return alloc;
}

u64 xstrlen(const char* str) {
    assert(str);
    return str ? strlen(str) : 0;
}

char* strf(const char* fmt, ...) {
    assert(fmt);
    va_list args;
    va_start(args, fmt);
    u64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);

    char* str = xmalloc(n);

    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);

    return str;
}

// Color Whell
static const char* colors[6] = {
    "\033[31m", // red
    "\033[32m", // green
    "\033[33m", // yellow
    "\033[34m", // blue
    "\033[35m", // magenta
    "\033[36m", // cyan,
};
static int counter = 0;
static int colors_count = 5;
const char* get_next_color(void) {
    ++counter;
    if (counter > colors_count)
        counter = 0;
    return colors[counter];
}
char* wrap_with_colored_parens(char* str) {
    const char* current_color = get_next_color();
    return strf("%s(\033[00m%s%s)\033[00m", current_color, str, current_color);
}
char* give_unique_color(char* str) {
    const char* current_color = get_next_color();
    return strf("%s%s\033[00m", current_color, str);
}
const char* get_unique_color(void) { return get_next_color(); }
//

//------------------------------------------------------------------------------
//                               Timing Utility Functions
//------------------------------------------------------------------------------

typedef struct StackNode StackNode;
struct StackNode {
    Timer data;
    StackNode* next;
};

StackNode* newNode(Timer data) {
    StackNode* stackNode = xmalloc(sizeof(StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

void push_stack(StackNode** root, Timer data) {
    StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

Timer pop_stack(StackNode** root) {
    StackNode* temp = *root;
    *root = (*root)->next;
    Timer popped = temp->data;
    return popped;
}

Timer peek(StackNode* root) { return root->data; }

f64 get_time(void) {
    f64 ms;
    time_t s;
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    s = spec.tv_sec;
    ms = spec.tv_nsec / 1.0e6;
    if (ms > 999.0) {
        ++s;
        ms = 0.0;
    }
    return ms;
}

static StackNode* timer_stack = NULL;
static Timer* timers = NULL;

Timer* get_timers(void) { return timers; }

void push_timer(const char* desc) {
    Timer tm;
    tm.ms = get_time();
    tm.desc = desc;
    push_stack(&timer_stack, tm);
}

void pop_timer(void) {
    Timer tm = pop_stack(&timer_stack);
    tm.ms = get_time() - tm.ms;
    sb_push(timers, tm);
}

//------------------------------------------------------------------------------
//                               Tests
//------------------------------------------------------------------------------
void utility_tests(void) {
    // These leak memory.

    // get_file_dir
    assert(strcmp(get_file_dir("./b/m.thi"), "./b/") == 0);
    assert(strcmp(get_file_dir("./b/m/m.thi"), "./b/m/") == 0);

    // get_file_ext
    assert(strcmp(get_file_ext("./b/m.thi"), "thi") == 0);
    assert(strcmp(get_file_ext("./b/m.thigegerg/o.hrifj"), "hrifj") == 0);

    // get_file_name
    assert(strcmp(get_file_name("./b/m.thi"), "m.thi") == 0);
    assert(strcmp(get_file_name("./b/m/hergergerg.thi"), "hergergerg.thi") ==
           0);

    // get_file_path_from_dir
    assert(strcmp(get_file_path_from_dir("./b/", "test.thi"), "./b/test.thi") ==
           0);
    assert(strcmp(get_file_path_from_dir("./b/b/", "test.thi"),
                  "./b/b/test.thi") == 0);
}
