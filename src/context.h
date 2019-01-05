#ifndef CONTEXT_H
#define CONTEXT_H

#include "stack.h"
#include "typedefs.h"
#include "value.h" // Value

typedef struct Context Context;

// The context holds information about the current state of the codegen phase.
struct Context
{
    Value* current_function;

    string output;

    Stack stack;

    i64 stack_index;

    // Label information
    i64 total_label_counter;
    i64 current_label_counter;

    char* temp_label0;
    char* temp_label1;

    char* label_continue_to;
    char* label_break_to;

    i32 next_available_reg_index;
};

void ctx_tests(void);
void ctx_init(Context* ctx);
void ctx_free(Context* ctx);
void ctx_push_label(Context* ctx);
void ctx_pop_label(Context* ctx);
void ctx_set_continue_label(Context* ctx, char* label);
void ctx_set_break_label(Context* ctx, char* label);
char* ctx_get_unique_label(Context* ctx);
#endif
