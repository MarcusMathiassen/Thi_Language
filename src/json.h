// Copyright (c) 2020 Marcus Mathiassen

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

#ifndef _JSON_H_
#define _JSON_H_

#include "utility.h"
#include "common.h"
#include "list.h"

typedef struct json_object_t  json_object_t;

typedef enum json_kind_t json_kind_t;
typedef enum json_number_kind_t json_number_kind_t;

enum json_kind_t {
    JSON_STRING,
    JSON_NUMBER,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_BOOLEAN,
    JSON_NULL,
    _JSON_KIND_COUNT_,
};

enum json_number_kind_t {
    JSON_NUMBER_INT,
    JSON_NUMBER_FLOAT,
};

struct json_object_t
{
    char* name;
    json_kind_t kind;
    union
    {
        struct { char* value; }  _string;
        struct {
            json_number_kind_t kind;
            union {
                s64 int_val;
                f64 flt_val;
            };
        }  _number;
        struct { List* siblings; } _object;
        struct { List* elements; } _array;
        struct { bool value; } _boolean;
    };
};


void json_test(void);
void json_free(json_object_t* object);

json_object_t* json_object_append(json_object_t* root, char* name, json_object_t* json);

char* json_to_str(json_object_t* json);
char* json_object_kind_to_str(json_kind_t kind);

json_object_t* make_json_string(char* value);
json_object_t* make_json_number_int(s64 value);
json_object_t* make_json_number_float(f64 value);
json_object_t* make_json_object(List* siblings);
json_object_t* make_json_array(List* elements);
json_object_t* make_json_boolean(bool value);
json_object_t* make_json_null();

#endif
