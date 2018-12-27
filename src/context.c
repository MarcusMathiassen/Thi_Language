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
    ctx->next_available_reg_index = 0;
}

void ctx_set_continue_label(Context* ctx, const char* label) { ctx->label_continue_to = label; }
void ctx_set_break_label(Context* ctx, const char* label) { ctx->label_break_to = label; }
void ctx_push_label(Context* ctx)
{
    assert(ctx);
    ++ctx->total_label_counter;
}

void ctx_pop_label(Context* ctx)
{
    assert(ctx);
}

const char* ctx_get_unique_label(Context* ctx)
{
    assert(ctx);
    ++ctx->current_label_counter;
    return strf("L%d", ctx->current_label_counter);
}

void ctx_tests(void)
{
    // Uniqueness check
    // Context ctx;
    // ctx_init(&ctx);

    // {
    //     ctx_push_label(&ctx);
    //     char* l1 = ctx_get_unique_label(&ctx);
    //     assert(strcmp(l1, "L11") == 0);
    //     ctx_pop_label(&ctx);
    //     char* l2 = ctx_get_unique_label(&ctx);
    //     assert(strcmp(l1, l2) == 0);
    // }
}
