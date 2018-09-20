#include "context.h"
#include "string.h"  // strf
#include "utility.h" // xmalloc
#include <assert.h>  // assert

Context* ctx_make(void) {
    Context* ctx = xmalloc(sizeof(Context));
    ctx->current_label_counter = 0;
    ctx->total_label_counter = 0;
    ctx->stack.index = 0;
    return ctx;
}

void ctx_push_label(Context* ctx) {
    assert(ctx);
    ++ctx->current_label_counter;
    ++ctx->total_label_counter;
}

void ctx_pop_label(Context* ctx) {
    assert(ctx);
    --ctx->current_label_counter;
}

char* ctx_get_unique_label(Context* ctx, const char* label_name) {
    assert(ctx);
    assert(label_name);
    char* str = strf("%s%d%d", label_name, ctx->total_label_counter,
                     ctx->current_label_counter);
    return str;
}
