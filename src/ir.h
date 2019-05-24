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
//                               ir.h
//------------------------------------------------------------------------------

#ifndef IR_H
#define IR_H

typedef enum {
    IR_NOP,     // does nothing.
    IR_ADD,     // add r0 r1 r2 => r0  r1 * r2
    IR_NEG,     // neg r0 r1 => additive inverse of value in r1
    IR_LOAD,    // load  <bytes> r0 r1   => gets <bytes> of bytes from memory stores in r1.
    IR_STORE,   // store <bytes> r0 r1   => stores <bytes> from r1 into r0. ex. 'store 1 r0 r1' will get 1 byte from r1 and put it into memory address of r0
    IR_EQ,      // eq r0 r1 r2 => if r1 == r2, r0 is 1, otherwise 0
    IR_IFNZ,    // if r1 label else label
    _IR_COUNT_
} IR_Kind;

typedef struct IR IR;
struct IR {
    IR_Kind kind;
    union {
    };
};


//------------------------------------------------------------------------------
//                                  IR Structures
//------------------------------------------------------------------------------


#endif
