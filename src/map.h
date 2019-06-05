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

#ifndef MAP_H
#define MAP_H

#include "typedefs.h"

//------------------------------------------------------------------------------
//                               map.h
//------------------------------------------------------------------------------

#ifdef NDEBUG
#define map_get(map, key) (p_map_get(map, key))
#define map_set(map, key, value) (p_map_set(map, key, value))
#define map_count(map) (p_map_count(map))
#else
#define map_get(map, key) (d_map_get(map, key, __FILE__, (char*)__func__, __LINE__))
#define map_set(map, key, value) (d_map_set(map, key, value, __FILE__, (char*)__func__, __LINE__))
#define map_count(map) (d_map_count(map, __FILE__, (char*)__func__, __LINE__))
#endif

typedef struct {
    char* key;
    void* value;
} Map_Element;

typedef struct {
    s64 table_size;
    s64 count;
    Map_Element* elements;
} Map;

Map map_create(void);
Map* make_map(void);
void map_tests(void);

s64 p_map_count(Map* map);
void* p_map_get(Map* map, char* key);
void* p_map_set(Map* map, char* key, void* value);
s64 d_map_count(Map* map, char* file, char* func, int line);
void* d_map_get(Map* map, char* key, char* file, char* func, int line);
void* d_map_set(Map* map, char* key, void* value, char* file, char* func, int line);
#endif
