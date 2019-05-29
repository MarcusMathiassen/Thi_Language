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

typedef struct Map Map;
typedef struct Map_Element Map_Element;

struct Map_Element {
    s64 key;
    void* data;
};

struct Map {
    s64 table_size;
    s64 size;
    Map_Element* data;
};

// clang-format off
Map*  make_map          (void);
void  map_tests         (void);
void* map_get           (Map* map, char* key);
void* map_set           (Map* map, char* key, void* value);
void* map_set_overwrite (Map* map, char* key, void* value);
// clang-format on
#endif
