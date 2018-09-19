#ifndef CONTEXT_H
#define CONTEXT_H

#include "typedefs.h"
#include "value.h" // Value


typedef struct Context Context;

// The context holds information about the current state of the codegen phase.
struct Context
{
    Value* current_function;

    // Stack information
    u64 stack_index;

    // Label information
    u64 total_label_counter;
    u64 current_label_counter;

    const char* label_continue_to;
    const char* label_break_to;

    // Global variables
};

Context* ctx_make(void);
void ctx_push_label(Context* ctx);
void ctx_pop_label(Context* ctx);
char* ctx_get_unique_label(Context* ctx, const char* label_name);

#endif
