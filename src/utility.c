#include <mach/mach_time.h> // mach_timebase_info_data_t, mach_absolute_time

#if DEV
    #define xmalloc(bytes) (_malloc(bytes, __FILE__, (char*)__func__, __LINE__))
    #define xcalloc(size, bytes) (_calloc(size, bytes, __FILE__, (char*)__func__, __LINE__))
    #define xrealloc(ptr, bytes) (_realloc(ptr, bytes, __FILE__, (char*)__func__, __LINE__))
    #define xstrlen(str) (_strlen(str, __FILE__, (char*)__func__, __LINE__))
#else
    #define xmalloc(bytes) malloc(bytes)
    #define xcalloc(size, bytes) calloc(size, bytes)
    #define xrealloc(ptr, bytes) realloc(ptr, bytes)
    #define xstrlen(str) strlen(str)
#endif

#if DEV
    #define xassert(EX) (void)((EX) || (_tassert(#EX, __FILE__, (char*)__func__, __LINE__, ""), 0))
    #define tassert(EX, FMT, ...) (void)((EX) || (_tassert(#EX, __FILE__, (char*)__func__, __LINE__, FMT, ##__VA_ARGS__), 0))
#else
    #define tassert(EX, FMT, ...)
    #define xassert(EX)
#endif

#if DEV
    #define debug(FMT, ...) info(FMT, ##__VA_ARGS__)
#else
    #define debug(FMT, ...)
#endif

// Forward decls
INTERNAL void* _malloc(s64 bytes, char* file, char* func, s64 line);
INTERNAL void* _calloc(s64 size, s64 bytes, char* file, char* func, s64 line);
INTERNAL void* _realloc(void* ptr, s64 bytes, char* file, char* func, s64 line);
INTERNAL void error(u8* fmt, ...);
INTERNAL u8* strf(u8* fmt, ...);

INTERNAL void
_tassert(char* expr_str, char* file, char* func, int line, char* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);
    char* str = xmalloc(n);
    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);
    error("[%s:%s:%s] ASSERT %s %s", (file), (func), (strf("%d", line)), (expr_str), str);
}

INTERNAL void*
_malloc(s64 bytes, char* file, char* func, s64 line)
{
    xassert(bytes != 0);
    void* alloc = malloc(bytes);
    if (!alloc) error("[%s:%s:%lld] malloc(%lld) failed", file, func, line, bytes);
    return alloc;
}

INTERNAL void*
_calloc(s64 size, s64 bytes, char* file, char* func, s64 line)
{
    xassert(size != 0);
    xassert(bytes != 0);
    void* alloc = calloc(size, bytes);
    if (!alloc) error("[%s:%s:%lld] calloc(%lld, %lld) failed", file, func, line, size, bytes);
    return alloc;
}

INTERNAL void*
_realloc(void* ptr, s64 bytes, char* file, char* func, s64 line)
{
    xassert(ptr);
    xassert(bytes != 0);
    void* alloc = realloc(ptr, bytes);
    if (!alloc) error("[%s:%s:%lld] realloc(%lld, %llu) failed", file, func, line, (u64)ptr, bytes);
    return alloc;
}
INTERNAL s64
_strlen(char* str, char* file, char* func, s64 line)
{
    xassert(str);
    s64 len = strlen(str);
    if (!len) error("[%s:%s:%lld] strlen(%s) failed", file, func, line, str);
    return len;
}

INTERNAL u8* 
strn(u8* start, u8* end)
{
    s64 len = end - start;
    u8* str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = '\0';
    return str;
}

INTERNAL u8*
strf(u8* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    s64 n = 1 + vsnprintf(0, 0, fmt, args);
    va_end(args);
    u8* str = xmalloc(n + 1);
    va_start(args, fmt);
    vsnprintf(str, n, fmt, args);
    va_end(args);
    str[n] = '\0';
    return str;
}

INTERNAL u8* vstrf(u8* fmt, va_list args)
{
    xassert(fmt);
    va_list args_count;
    va_copy(args_count, args);
    s64 n = 1 + vsnprintf(0, 0, fmt, args_count);
    va_end(args_count);
    u8* str = xmalloc(n + 1);
    vsnprintf(str, n, fmt, args);
    str[n] = '\0';
    return str;
}

INTERNAL char* get_file_extension(char* filename)
{
    xassert(filename);
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
        return NULL; // we didnt find any
    }
    s64 start_of_ext = len - last_dot_pos;
    char* str = xmalloc(start_of_ext + 2);
    memcpy(str, filename + len - start_of_ext, start_of_ext);
    str[start_of_ext] = 0;
    return str;
}

INTERNAL char* remove_file_extension(char* filename)
{
    xassert(filename);
    s64 len = strlen(filename);
    s64 i = 0;
    while (i < len) {
        if (filename[len - (i)] == '.') break;
        ++i;
    }
    if (i == 0 || i == len) {
        return NULL; // we didnt find any
    }
    char* str = xmalloc(len - i + 1);
    memcpy(str, filename, len - i);
    str[len - i] = 0;
    return str;
}

INTERNAL char* get_file_directory(char* filename)
{
    xassert(filename);
    s64 len = strlen(filename);
    while (len > 0) {
        if (filename[len] == '/') break;
        --len;
    }
    if (len == 0) {
        return NULL; // we didnt find any
    }
    ++len; // we preserve the '/'
    char* str = xmalloc(len + 1);
    memcpy(str, filename, len);
    str[len] = 0;
    return str;
}

INTERNAL char* get_file_name(char* filename)
{
    xassert(filename);
    s64 len = strlen(filename);
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
    return str;
}

INTERNAL u8*
get_file_content(u8* filename)
{
    u8* buffer = NULL;
    FILE* f = fopen(filename, "r");
    if (f)
    {
        fseek(f, 0, 2);
        u64 len = ftell(f);
        rewind(f);
        buffer = xmalloc(len + 1);
        fread(buffer, 1, len, f);
        buffer[len] = '\0';
        fclose(f);
    } else error("error reading file: %s", filename);
    return buffer;
}

INTERNAL void
write_to_file(u8* filename, u8* buffer)
{
    xassert(filename);
    xassert(buffer);
    FILE* f = fopen(filename, "w");
    if (!f)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fputs(buffer, f);
    fclose(f);
}

INTERNAL void
info(u8* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RGB_GRAY);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);   
}

INTERNAL void
warning(u8* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", YELLOW);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
}

INTERNAL void
success(u8* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", GREEN);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
}

INTERNAL void
error(u8* fmt, ...)
{
    xassert(fmt);
    va_list args;
    va_start(args, fmt);
    printf("%s", RED);
    vprintf(fmt, args);
    puts(RESET);
    va_end(args);
    exit(1);
}


INTERNAL u8*
time_with_suffix(u64 n)
{
    u64 m = (n >= HOURS(1ULL) ? 
            n / HOURS(1ULL) : n >= MINUTES(1ULL) 
            ? n / MINUTES(1ULL) :  n >= SECONDS(1ULL) 
            ? n / SECONDS(1ULL) :  n >= MILLISECONDS(1ULL) 
            ? n / MILLISECONDS(1ULL) : n >= MICROSECONDS(1ULL) 
            ? n / MICROSECONDS(1ULL) :  n >= NANOSECONDS(1ULL) 
            ? n / NANOSECONDS(1ULL) : n);

    u8* suffix = (n >= HOURS(1ULL) ? HOURS_SUFFIX
        : n >= MINUTES(1ULL) ? MINUTES_SUFFIX
        : n >= SECONDS(1ULL) ? SECONDS_SUFFIX
        : n >= MILLISECONDS(1ULL) ? MILLISECONDS_SUFFIX
        : n >= MICROSECONDS(1ULL) ? MICROSECONDS_SUFFIX
        : n >= NANOSECONDS(1ULL) ? NANOSECONDS_SUFFIX
        : "");
    
    return strf("%llu%s", m, suffix);
}

INTERNAL u8*
size_with_suffix(u64 n)
{
    u64 m = (n >= GIGABYTES(1ULL) ? n / GIGABYTES(1ULL) : n >= MEGABYTES(1ULL) ? n / MEGABYTES(1ULL) :  n >= KILOBYTES(1ULL) ? n / KILOBYTES(1ULL) :  n >= BYTES(1ULL) ? n / BYTES(1ULL) : n);
    u8* suffix = (n >= GIGABYTES(1ULL) ? GIGABYTE_SUFFIX
        : n >= MEGABYTES(1ULL) ? MEGABYTE_SUFFIX
        :  n >= KILOBYTES(1ULL) ? KILOBYTE_SUFFIX
        :  n >= BYTES(1ULL) ? BYTE_SUFFIX
        : "");
    return strf("%llu%s", m, suffix);
}

GLOBAL_VARIABLE char* colors[] =
{
    "\033[31m", // red
    "\033[32m", // green
    "\033[33m", // yellow
    "\033[34m", // blue
    "\033[35m", // magenta
    "\033[36m", // cyan,
};

GLOBAL_VARIABLE u8 color_iterator = 0;
INTERNAL u8*
give_unique_color(char* str)
{
    xassert(str);
    char* current_color = colors[color_iterator++ % CONST_ARRAY_LENGTH(colors)];
    xassert(current_color);
    return strf("%s%s\033[00m", current_color, str);
}

#define ucolor(x) give_unique_color(x)

INTERNAL u64 get_time()
{
    mach_timebase_info_data_t info;
    if (mach_timebase_info(&info) != KERN_SUCCESS) abort();
    return (mach_absolute_time() * info.numer / info.denom);
}

INTERNAL char* table_entry(char* left, char* right)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    s64 len = xstrlen(left);
    s64 ms_l = xstrlen(right);
    s64 padding = w.ws_col - len - ms_l;
    return strf("%s%*s%s", left, padding, "", right);
}

INTERNAL char* str_replace_center(char* str, char* into)
{
    s64 slen = xstrlen(str);
    s64 ilen = xstrlen(into);

    s64 imid = ilen/2;
    s64 i_starting_point = imid - slen/2;

    for (s64 i = i_starting_point; i < slen+i_starting_point; ++i) {
        into[i] = str[i-i_starting_point];
    }

    return into;
}

INTERNAL char* pad_out_full_width(char ch)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    char* str = xmalloc(w.ws_col + 1);
    foreach(i, w.ws_col)
        str[i] = ch;
    str[w.ws_col] = '\0';
    return str;
}

INTERNAL char* align_center(char* str)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    s64 len = xstrlen(str);
    s64 middle = w.ws_col / 2;
    s64 padding = middle - len / 2;

    return strf("%*s%s", padding, "", str);    
}
