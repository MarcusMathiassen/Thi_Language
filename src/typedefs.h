#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef     int8_t    s8;
typedef     int16_t   s16;
typedef     int32_t   s32 ;
typedef     int64_t   s64;

typedef     uint8_t   u8;
typedef     uint16_t  u16;
typedef     uint32_t  u32;
typedef     uint64_t  u64;

typedef     float     f32;
typedef     double    f64;

typedef     s8        bool;

#define     true      1
#define     false     0

#define THI_MEM_DEBUG
#ifdef THI_MEM_DEBUG
#define malloc(n) _malloc(n, __FILE__, __LINE__)
#define realloc(n, m) _realloc(n, m, __FILE__, __LINE__)
#define calloc(n, m) _calloc(n, m, __FILE__, __LINE__)
#endif


#endif
