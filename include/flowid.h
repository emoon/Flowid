#pragma once
#include <stddef.h>
#include <stdint.h>
#include "flowid_types.h"

// TODO: Temporary
#define FLD_SUPPORT_MALLOC

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Flowid (Flowi Drawing) takes insperation from the following sources: NanoVG, HTML canvas API, but also diveates
/// a bit from them.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Create and destuction
///
/// Flowid supports working within a fully contained memory space meaning that the memory usage of the library is
/// is fully predictable. This is useful for embedding the library in other applications with fixed memory constrains
/// such as embedded systems. It's also possible to create with a custom allocator, or optionally with malloc as
/// the default allocator. In this mode Flowid will try to alloc more memory if it needs it. If it can't it will
/// try to dump out memory info using a user supplied callback, otherwise it will use printf and print.
///

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Frames and drawing
///
/// The way Flowid works is that it tries to deferre all the generation to the end of the frame. This means means
/// that the performance of the API when used in the client code is a predictable as possible. The majority of
/// API will simply copy the input data to a command buffer to be used later. There are some cases (like
/// fld_text_get_size(...)) That has to do some more work as as the bounds of the text has to be calculated to return
/// the bounds.

/// Start of a new frame. The device_pixel_ratio is the upscaling ratio (this us usually refered to as High DPI) and
/// depends on whan your screen is set to. Usually the windowing system you are using should be able to retrive this
/// value.
void fld_frame_begin(struct FldContext* ctx, int window_width, int window_height, float device_pixel_ratio);

/// Ends the current frame and generates the vertex buffers, rendering commands, etc
void fld_frame_end(struct FldContext* ctx);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Paths
///
/// Drawing starts with begin_path and ends with end_path(). It's possible to add sub-paths within an existing path
/// as well.
/// After defining a path, you can add points to the path by calling move_to, line_to, bezier_to, arc_to, quad_to,
/// functions. At the end stroke_path() is called to draw the path. It's also possible to fill the path using
/// fill_path().
///
/// Along with using these functions you can alse use fld_full_path() that takes a struct with the data to draw. Using
/// this function can be a bit more efficient depending on how many calls that needs to be made to the regular API.
///

/// Starts a new path
void fld_path_begin(struct FldContext* ctx);

/// Closes the path and draws it.
void fld_path_close(struct FldContext* ctx);

/// Starts a new sub path at a given point (x, y)
void fld_path_move_to(struct FldContext* ctx, float x, float y);

/// Draws a line from the previous "move_to" point
void fld_path_line_to(struct FldContext* ctx, float x, float y);

/// Draws a rectangle with given position and size.
void fld_path_rect(struct FldContext* ctx, float x, float y, float w, float h);

/// Addes a rounded rectangle with given position, size and radius
void fld_path_round_rect(struct FldContext* ctx, float x, float y, float width, float height, float radius);

// Adds an arc path at the corner defined by the last path point, and two specified points.
void fld_path_arc_to(struct FldContext* ctx, float x1, float y1, float x2, float y2, float radius);

/// Adds an ellipse sub-path
void fld_path_ellipse(struct FldContext* ctx, float cx, float cy, float rx, float ry);

/// Adds an Quadratic Bezier curve sub-path.
void fld_path_quadratic_curve_to(struct FldContext* ctx, float x1, float y1, float x2, float y2);

/// Adds a Bezier curve sub-path. It's recommended to use Quadratic Bezier curves instead, as they are more efficient.
void fld_path_bezier_curve_to(struct FldContext* ctx, float x1, float y1, float x2, float y2, float x3, float y3);

/// Generates a stroke of the current path with the current set style.
void fld_path_stroke(struct FldContext* ctx);

// Fills the path with the current style.
void fld_path_fill(struct FldContext* ctx);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Images
//
// to load jpg, png, psd, tga, pic and gif files to be used for rendering.
// In addition you can upload your own image. The image loading is provided by stb_image.
// image_flags are a bitmask of the values in FldImageFlags
//

// Creates image by loading it from local file. Handle to image is returned and 0 indicates a failure.
FldImage fld_image_create(struct FldContext* ctx, FldText filename, uint32_t image_flags);

// Destroys an existing image
void fld_image_delete(struct FldContext* ctx, FldImage handle);

// Creates a image from memory. Handle to image is returned and 0 indicates a failure.
// It's expected that this memory is of the type of the supported image types by stb_image
FldImage fld_image_create_mem(struct FldContext* ctx, uint8_t* data, size_t size, uint32_t image_flags);

// Creates a image from memory. Handle to image is returned and 0 indicates a failure.
// It's expected that this memory is of the type of the supported image types by stb_image
FldImage fld_image_create_mem(struct FldContext* ctx, uint8_t* data, size_t size, uint32_t image_flags);

// Returns the size of an image. If handle is invalid then {0, 0} is returned.
FldSize fld_image_size(struct FldContext* ctx, FldImage handle);

/// Draws an image at the given position.
void fld_image_draw(struct FldContext* ctx, FldImage handle, float x, float y, int width, int height);

/// Draws an image at the given position with more details. This is the same as fld_image_draw(...) but it allows
/// you to specify the sub-rectangle of the image to draw.
void fld_image_draw_rect(struct FldContext* ctx, FldImage handle, FldRect dest, FldRect image_src);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Text related functions
//
// Text drawing is supported from .ttf files. Flowid uses Freetype2 (https://www.freetype.org/) to load the .ttf files.
// Freetype2 was selected (even if being a more complicated library) because it generates better results.
//

/// Creates a font from a .ttf file. 0 is returned if the font could not be loaded.
/// Notice that if FLOWID_STDIO isn't defined this function will not be available and will fail.
FldFont fld_font_create(struct FldContext* ctx, FldText filename);

/// Creates a font from a .ttf in memory. 0 is returned if the font could not be loaded.
FldFont fld_font_create_mem(struct FldContext* ctx, const uint8_t* mem, size_t size);

/// Destroys an existing font
void fld_font_destroy(struct FldContext* ctx, FldFont handle);

/// Set the size of the font.
void fld_font_set_size(struct FldContext* ctx, FldFont handle, float size);

/// Set  the active font to be used for drawing text.
void fld_text_set_font(struct FldContext* ctx, FldFont handle);

/// Draw text at given position with active font
void fld_text_draw(struct FldContext* ctx, FldFont font, FldText text, float x, float y);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Utility functions
///
/// Various utility functions that are not directly related to drawing, but can be useful to have.
///

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
typedef struct FldAllocator {
    /// User data is passed to alloc/realloc/free functions. This can be anything the user desires
    void* user_data;
    /// Allocate a block of memory at a specific alignment.
    /// This call should return a pointer to the allocated memory or NULL if the allocation failed.
    void* (*alloc)(void* user_data, size_t size, size_t align);
    /// Reallocate a block of memory at a specific alignment. Same rules as for alloc applies
    void* (*realloc)(void* user_data, void* old_ptr, size_t old_size, size_t size, size_t align);
    /// Free a block of memory.
    void (*free)(void* user_data, void* ptr);
} FldAllocator;
