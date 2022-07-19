#pragma once
#include <stddef.h>
#include <stdint.h>
#include "flowid_types.h"

struct FldAllocator;
struct FldContext;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Flowid (Flowi Drawing) takes insperation from the following sources: NanoVG, HTML canvas API, but also diveates
/// a bit from them.

/// Create a context for drawing with fixed memory.
struct FldContext* fld_create_with_fixed_memory(uint8_t* data, size_t data_size);

#if defined(FLD_SUPPORT_MALLOC)
// Create a context with malloc as backing allocator
struct FldContext* fld_create_with_malloc_allocator();
#endif

/// Create context with custom allocator
struct FldContext* fld_create_with_allocator(struct FldAllocator* allocator);

/// Destroy a context.
void fld_destroy(struct FldContext* ctx);

FldColor fld_hsl_to_rgb_ref(float h, float s, float l, float a);
FldColor fld_hsl_to_rgb(float h, float s, float l, float a);

/// Get current memory usage in bytes.
size_t fld_get_memory_usage(struct FldContext* ctx);

/*
typedef enum FldShapeType {
    FldShapeType_Rect,
    FldShapeType_Circle,
    FldShapeType_Line,
    FldShapeType_Triangle,
    FldShapeType_Polygon,
    FldShapeType_Path,
    FldShapeType_Text,
    FldShapeType_Image,
    FldShapeType_Max
} FldShapeType;

typedef struct FldShape {
    FldShapeType shape_type;
    union {
        struct {
            float x, y, w, h;
        } rect;
        struct {
            float x, y, r;
        } circle;
        struct {
            float x1, y1, x2, y2;
        } line;
        struct {
            float x1, y1, x2, y2, x3, y3;
        } triangle;
        struct {
            float* points;
            size_t points_count;
        } polygon;
        struct {
            float* points;
            size_t points_count;
        } path;
        struct {
            float x, y, font_size, line_height, letter_spacing, word_spacing;
            const char* text;
        } text;
        struct {
            float x, y, w, h;
            const uint8_t* data;
            size_t data_size;
        } image;
    };
} FldShape;
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Begins drawing of a shape

// void fld_begin_shape(struct FldContext* ctx, FldShape shape);

/*
inline void test_draw(struct FldContext* ctx) {
    // clang-format off
    FldShape shape {
        .shape_type = FldShapeType_Rect,
        .rect = {.x = 0, .y = 0, .w = 100, .h = 100}
    };
    // clang-format on

    fld_begin_shape(ctx, shape);
}
*/

// Allocator interface required to be implemented by the user when using [fld_create_with_allocator]
struct FldAllocator {
    /// Allocate a block of memory at a specific alignment.
    /// This call should return a pointer to the allocated memory or NULL if the allocation failed.
    void* (*alloc)(void* user_data, size_t size, size_t align);
    /// Reallocate a block of memory at a specific alignment. Same rules as for alloc applies
    void* (*realloc)(void* user_data, void* old_ptr, size_t size, size_t align);
    /// Free a block of memory.
    void (*free)(void* user_data, void* ptr);
    /// User data is passed to alloc/realloc/free functions. This can be anything the user desires
    void* user_data;
};
