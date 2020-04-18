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

#include "json.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>
#include "string.h"

void json_test(void)
{
    // {
    //     "firstname": "Marcus",
    //     "lastname": "Mathiassen",
    //     "age": 25,
    //     "married": false,
    //     "callback": null,
    //     "address": {
    //         "code": 9408,
    //         "city": "Harstad",
    //         "county": "Troms",
    //     },
    //     "colors": ["red", "green", "blue"],
    //     "emptyObject": {},
    //     "emptyArray": [],
    // }

    json_object_t* address = make_json_object(NULL);
    json_object_append(address, "code", make_json_number_int(9408));
    json_object_append(address, "city", make_json_string("Harstad"));
    json_object_append(address, "county", make_json_string("Troms"));

    json_object_t* colors = make_json_array(NULL);
    json_object_append(colors, NULL, make_json_string("red"));
    json_object_append(colors, NULL, make_json_string("green"));
    json_object_append(colors, NULL, make_json_string("blue"));

    json_object_t* root = make_json_object(NULL);
    json_object_append(root, "firstname", make_json_string("Marcus"));
    json_object_append(root, "lastname", make_json_string("Mathiassen"));
    json_object_append(root, "age", make_json_number_int(25));
    json_object_append(root, "married", make_json_boolean(false));
    json_object_append(root, "callback", make_json_null());
    json_object_append(root, "address", address);
    json_object_append(root, "emptyObject", make_json_object(NULL));
    json_object_append(root, "emptyArray", make_json_array(NULL));

    json_free(root);

    // error("%s", json_to_str(root));
}

static void json_free_object (json_object_t* object);
static void json_free_array  (json_object_t* object);
void json_free(json_object_t* object)
{
    switch (object->kind)
    {
        case JSON_STRING:   break;
        case JSON_NUMBER:   break;
        case JSON_OBJECT:   json_free_object(object);  break;
        case JSON_ARRAY:    json_free_array(object);   break;
        case JSON_BOOLEAN:  break;
        case JSON_NULL:     break;
    }

    free(object);
}

static void json_free_object (json_object_t* object)
{
    list_foreach(object->_object.siblings) {
        json_free(it->data);
    }
}
static void json_free_array  (json_object_t* object)
{
    list_foreach(object->_array.elements) {
        json_free(it->data);
    }
}

json_object_t* json_object_append(json_object_t* root, char* name, json_object_t* json)
{
    xassert(json);
    xassert(root->kind == JSON_OBJECT || root->kind == JSON_ARRAY);
    json->name = name;
    switch (root->kind)
    {
        case JSON_OBJECT: list_append(root->_object.siblings, json); break;
        case JSON_ARRAY:  list_append(root->_array.elements, json); break;
    }
    return root;
}


char* json_object_kind_to_str(json_kind_t kind)
{
    xassert(kind >= 0 && kind < _JSON_KIND_COUNT_);
    switch (kind)
    {
        case JSON_STRING:  return "JSON_STRING";
        case JSON_NUMBER:  return "JSON_NUMBER";
        case JSON_OBJECT:  return "JSON_OBJECT";
        case JSON_ARRAY:   return "JSON_ARRAY";
        case JSON_BOOLEAN: return "JSON_BOOLEAN";
        case JSON_NULL:    return "JSON_NULL";
    }
    UNREACHABLE;
    return NULL;
}


typedef struct {
    string* str;
} String_Context;


static void _json_to_str_string(String_Context* ctx, json_object_t* json);
static void _json_to_str_number(String_Context* ctx, json_object_t* json);
static void _json_to_str_object(String_Context* ctx, json_object_t* json);
static void _json_to_str_array(String_Context* ctx, json_object_t* json);
static void _json_to_str_boolean(String_Context* ctx, json_object_t* json);
static void _json_to_str_null(String_Context* ctx, json_object_t* json);


static void (*json_to_str_transitions[])(String_Context*, json_object_t*) = {
    [JSON_STRING] = _json_to_str_string,
    [JSON_NUMBER] = _json_to_str_number,
    [JSON_OBJECT] = _json_to_str_object,
    [JSON_ARRAY] = _json_to_str_array,
    [JSON_BOOLEAN] = _json_to_str_boolean,
    [JSON_NULL] = _json_to_str_null,
};

static char* _json_to_str(String_Context* ctx, json_object_t* json);

char* json_to_str(json_object_t* json)
{
    String_Context ctx;
    ctx.str = make_string("");
    return _json_to_str(&ctx, json);
}

static char* _json_to_str(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    void (*func)(String_Context*, json_object_t*) = (*json_to_str_transitions[json->kind]);
    tassert(func, "_json_to_str missing callback for %s", json->kind);
    (*func)(ctx, json);
    return string_data(ctx->str);
}

static void _json_to_str_string(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    string* s = ctx->str;
    string_append_f(s, "\"%s\"", json->_string.value);
}
static void _json_to_str_number(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    string* s = ctx->str;
    switch (json->_number.kind)
    {
        case JSON_NUMBER_INT:
            string_append_f(s, "%d", json->_number.int_val);
            break;
        case JSON_NUMBER_FLOAT:
            string_append_f(s, "%f", json->_number.flt_val);
            break;
    }
}
static void _json_to_str_object(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    string* s = ctx->str;
    string_append(s, "{\n");
    list_foreach(json->_object.siblings) {
        json_object_t* sibling = it->data;
        xassert(sibling->name);
        string_append_f(s, "\"%s\": ", sibling->name);
        _json_to_str(ctx, sibling);
        if (it->next) string_append(s, ",\n");
    }
    string_append(s, "}\n");
}
static void _json_to_str_array(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    string* s = ctx->str;
    string_append(s, "[\n");
    list_foreach(json->_array.elements) {
        _json_to_str(ctx, it->data);
        if (it->next) string_append(s, ",\n");
    }
    string_append(s, "]\n");
}
static void _json_to_str_boolean(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    string* s = ctx->str;
    string_append_f(s, "%s", json->_boolean.value ? "true" : "false");
}
static void _json_to_str_null(String_Context* ctx, json_object_t* json)
{
    xassert(ctx && json);
    string* s = ctx->str;
    string_append(s, "null");
}

static json_object_t* make_json(json_kind_t kind)
{
    json_object_t* j = malloc(sizeof(json_object_t));
    j->kind = kind;
    j->name = NULL;
    return j;
}

json_object_t* make_json_string(char* value)
{
    json_object_t* j = make_json(JSON_STRING);
    j->_string.value = value;
    return j;
}
json_object_t* make_json_number_int(s64 value)
{
    json_object_t* j = make_json(JSON_NUMBER);
    j->_number.kind = JSON_NUMBER_INT;
    j->_number.int_val = value;
    return j;
}
json_object_t* make_json_number_float(f64 value)
{
    json_object_t* j = make_json(JSON_NUMBER);
    j->_number.kind = JSON_NUMBER_FLOAT;
    j->_number.flt_val = value;
    return j;
}
json_object_t* make_json_object(List* siblings)
{
    json_object_t* j = make_json(JSON_OBJECT);
    if (!siblings) siblings = make_list();
    j->_object.siblings = siblings;
    return j;
}
json_object_t* make_json_array(List* elements)
{
    json_object_t* j = make_json(JSON_ARRAY);
    if (!elements) elements = make_list();
    j->_array.elements = elements;

    return j;
}
json_object_t* make_json_boolean(bool value)
{
    json_object_t* j = make_json(JSON_BOOLEAN);
    j->_boolean.value = value;
    return j;
}
json_object_t* make_json_null()
{
    json_object_t* j = make_json(JSON_NULL);
    return j;
}
