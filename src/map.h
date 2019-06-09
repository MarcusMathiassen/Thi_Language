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

//------------------------------------------------------------------------------
//                               map.h
//------------------------------------------------------------------------------

#ifndef MAP_H
#define MAP_H

#include "typedefs.h"

typedef struct {
    char* key;
    void* value;
} Map_Element;

typedef struct {
    s64 table_size;
    s64 count;
    Map_Element* elements;
} Map;

#define map_foreach(map) for (Map_Element* it = map->elements; it != &(map->elements[map->table_size]); ++it) if (it->key)

Map* make_map(void);
void map_destroy(Map* map);
Map* make_map_with_initial_size(s64 initial_size);
s64 map_count(Map* map);
void map_tests(void);

void* p_map_set(Map* map, char* key, void* value);
void* p_map_get(Map* map, char* key);
void* d_map_set(Map* map, char* key, void* value, char* file, char* func, int line);
void* d_map_get(Map* map, char* key, char* file, char* func, int line);

#ifdef NDEBUG
#define map_set(m, k, v) p_map_set(m, k, v)
#define map_get(m, k) p_map_get(m, k)
#else
#define map_set(m, k, v) d_map_set(m, k, v, __FILE__, (char*)__func__, __LINE__)
#define map_get(m, k) d_map_get(m, k, __FILE__, (char*)__func__, __LINE__)
#endif


#endif
