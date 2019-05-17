#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stddef.h>
#include <stdint.h>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef s8 bool;

#define ERROR_UNHANDLED_KIND(kind) error("[%s:%s:%s] Unhandled case '%s'", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), give_unique_color((char*)__LINE__), give_unique_color(kind));
#define UNREACHABLE error("[%s:%s:%s] UNREACHABLE HIT", give_unique_color((char*)__FILE__), give_unique_color((char*)__func__), give_unique_color((char*)__LINE__));

#define true 1
#define false 0

#endif
