//
//  stack.h
//  Thi_Language
//
//  Created by Marcus Mathiassen on 19/09/2018.
//  Copyright © 2018 Marcus Mathiassen. All rights reserved.
//

#ifndef stack_h
#define stack_h

#include "typedefs.h"

typedef struct Stack Stack;

// Represents the idea of a stack.
// The stack is used to hold variables in the program
// You allocate stack by adding and subtracting the stackpointer

struct Stack {
  u64 index;
  u64 allocated_size;
  u64 free_space;
};

#endif /* stack_h */
