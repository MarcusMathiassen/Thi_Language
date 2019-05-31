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

#ifndef CONSTANTS_H
#define CONSTANTS_H


#define COMPILER_VERSION "0.1"

#define RESET "\033[0m"
#define GRAY "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RGB_GRAY "\033[38;2;110;110;110;m"
#define RGB_WHITE "\033[38;2;255;255;255;m"

#define DEFAULT_FOR_IN_ITERATOR_NAME "it"
#define DEFAULT_INDENT_LEVEL 4

#define DEFAULT_POINTER_SIZE 8
#define DEFAULT_ENUM_SIZE 8

#define DEFAULT_INT_BYTE_SIZE 8
#define DEFAULT_FLOAT_BYTE_SIZE 8

#define DEFAULT_INT_TYPE_AS_STRING (DEFAULT_INT_BYTE_SIZE == 8) ? "s64" : "s32"
#define DEFAULT_BIG_INT_TYPE_AS_STRING "s64"
#define DEFAULT_FLOAT_TYPE_AS_STRING (DEFAULT_FLOAT_BYTE_SIZE == 8) ? "f64" : "f32"
#define DEFAULT_BIG_FLOAT_TYPE_AS_STRING "f64"

#define X64_ASM_MACOS_STACK_PADDING 16 // is 16 on macOS

#define DEFAULT_FUNCTION_ENTRY_LABEL_NAME ".begin"
#define DEFAULT_FUNCTION_END_LABEL_NAME ".end"

#define AST_REF_LIST_STARTING_ALLOC 100
#define TYPE_REF_LIST_STARTING_ALLOC 100
#define INTERN_ARRAY_STARTING_ALLOC 100
#define TOKEN_ARRAY_STARTING_ALLOC 100

#define STRING_STARTING_ALLOC 16

#define PHI 1.61803398875

#endif
