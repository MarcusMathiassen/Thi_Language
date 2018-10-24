#include "context.h"
#include "string.h"  // strf
#include "utility.h" // xmalloc
#include <assert.h>  // assert
#include <string.h>  // strcmp

void ctx_init(Context* ctx)
{
    ctx->current_label_counter = 0;
    ctx->total_label_counter = 0;
    ctx->stack_index = 0;
}

void ctx_push_label(Context* ctx)
{
    assert(ctx);
    ++ctx->current_label_counter;
    ++ctx->total_label_counter;
}

void ctx_pop_label(Context* ctx)
{
    assert(ctx);
    --ctx->current_label_counter;
}

char* ctx_get_unique_label(Context* ctx, const char* label_name)
{
    assert(ctx);
    assert(label_name);
    char* str = strf("%s%d%d", label_name, ctx->total_label_counter, ctx->current_label_counter);
    return str;
}

void ctx_tests(void)
{
    // Uniqueness check
    Context ctx;
    ctx_init(&ctx);

    // {
    //     ctx_push_label(&ctx);
    //     char* label = ctx_get_unique_label(&ctx, "a");
    //     assert(strcmp(label, "a1") == 0);
    //     info("a1 %s", label);
    //     {
    //         ctx_push_label(&ctx);
    //         label = ctx_get_unique_label(&ctx, "a");
    //         assert(strcmp(label, "a2") == 0);
    //         info("a2 %s", label);
    //         ctx_pop_label(&ctx);
    //     }
    //     label = ctx_get_unique_label(&ctx, "a");
    //     info("a1 %s", label);
    //     assert(strcmp(label, "a1") == 0);
    //     ctx_pop_label(&ctx);
    // }
}