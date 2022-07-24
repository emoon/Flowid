#include "flowid_types.h"
#include "include/flowid.h"
#include "flowid_private.h"

// TODO: Move to configure
#define FLD_ESTIMATED_QUADRATIC_CURVE_COUNT 16

#if defined(FLD_SUPPORT_MALLOC)

#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void* malloc_aligned(void* user_data, size_t size, size_t align) { 
    (void)user_data;

    void* result = NULL;
#ifdef _MSC_VER 
    result = _aligned_malloc(size, align);
#else 
    if (posix_memalign(&result, align, size)) {
        result = NULL;
    }
#endif
    return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void* malloc_realloc(void* user_data, void* old_ptr, size_t old_size, size_t size, size_t align) {
    (void)user_data;
    // as there is no realloc for malloc we need to do this manually
    void* new_data = malloc_aligned(user_data, size, align);
    // TODO: Handle when we run out of memory
    memcpy(new_data, old_ptr, old_size);
    free(old_ptr);
    return new_data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void malloc_free(void* user_data, void* ptr) {
    free(ptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static struct FldAllocator s_malloc_allocator = {
    NULL,
    malloc_aligned,
    malloc_realloc,
    malloc_free,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FldContext* fld_create_with_malloc_allocator() {
    return fld_create_with_allocator(&s_malloc_allocator);
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Create context with custom allocator

struct FldContext* fld_create_with_allocator(struct FldAllocator* allocator) {
    FldContext* ctx = Allocator_alloc(allocator, FldContext);
    ctx->allocator = *allocator;
    return ctx;

    /*
    LinearAllocator_create_with_allocator(
        &ctx->curves_allocator, 
        "curves", &ctx->allocator, 
        FLD_ESTIMATED_QUADRATIC_CURVE_COUNT * sizeof(SoAQuadraticCurve), true);
    */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fld_frame_begin(struct FldContext* ctx, int window_width, int window_height, float device_pixel_ratio) {


}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Ends the current frame and generates the vertex buffers, rendering commands, etc

void fld_frame_end(struct FldContext* ctx) {
    //LinearAllocator_rewind(&ctx->curves_allocator);
    //ctx->current_quadratic_curves = LinearAllocator_alloc(&ctx->curves_allocator, SoAQuadraticCurve);
    //ctx->quadratic_curves_count = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Starts a new path

void fld_path_begin(struct FldContext* ctx) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Closes the path

void fld_path_close(struct FldContext* ctx) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Starts a new sub path at a given point (x, y)

void fld_path_move_to(struct FldContext* ctx, float x, float y) {
    MoveToCommand* cmd = CommandBuffer_alloc(&ctx->commands, FLD_PATH_MOVE_TO, MoveToCommand);
    *cmd = (MoveToCommand){ x, y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a line from the previous "move_to" point

void fld_path_line_to(struct FldContext* ctx, float x, float y) {
    LineToCommand* cmd = CommandBuffer_alloc(&ctx->commands, FLD_PATH_LINE_TO, LineToCommand);
    *cmd = (LineToCommand){ x, y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Draws a rectangle with given position and size.

void fld_path_rect(struct FldContext* ctx, float x, float y, float w, float h) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Addes a rounded rectangle with given position, size and radius

void fld_path_round_rect(struct FldContext* ctx, float x, float y, float width, float height, float radius) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Adds an arc path at the corner defined by the last path point, and two specified points.

void fld_path_arc_to(struct FldContext* ctx, float x1, float y1, float x2, float y2, float radius) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Adds an ellipse sub-path

void fld_path_ellipse(struct FldContext* ctx, float cx, float cy, float rx, float ry) {

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Adds an Quadratic Bezier curve sub-path {

void fld_path_quadratic_curve_to(struct FldContext* ctx, float x1, float y1, float x2, float y2) {
    QadraticCurveToCommand* cmd = CommandBuffer_alloc(&ctx->commands, FLD_PATH_QUDRATIC_CURVE_TO, 
                                                      QadraticCurveToCommand);
    *cmd = (QadraticCurveToCommand){ x1, y1, x2, y2 };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Adds a Bezier curve sub-path. It's recommended to use Quadratic Bezier curves instead, as they are more efficient.

void fld_path_bezier_curve_to(struct FldContext* ctx, float x1, float y1, float x2, float y2, float x3, float y3) {
    CubicCurveToCommand* cmd = CommandBuffer_alloc(&ctx->commands, FLD_PATH_CUBIC_CURVE_TO, CubicCurveToCommand);
    *cmd = (CubicCurveToCommand){ x1, y1, x2, y2, x3, y3 };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Generates a stroke of the current path with the current set style.

void fld_path_stroke(struct FldContext* ctx) {

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fills the path with the current style.

void fld_path_fill(struct FldContext* ctx) {


}


/*
FldColor fld_hsl_to_rgb(float h, float s, float l, float a) {
    FldColor output[16];
    fld_hsl_to_rgb_ispc((struct $anon0*)&output, h, s, l, a);
    return output[0];
}
*/

