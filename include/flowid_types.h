#pragma once

#include <stdint.h>
#include <stddef.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    float r, g, b, a;
} FldColor;

typedef struct {
    int x, y;
} FldSize;

typedef struct {
    int x, y, width, height;
} FldRect;

/// These flags has been based on the flags from NanoVG by Mikko Mononen.
enum FldImageFlags {
    /// Generate mipmaps during creation of the image.
    FLD_IMAGE_GENERATE_MIPMAPS = 1 << 0,
    /// Repeat image in X direction.
    FLD_IMAGE_REPEATX = 1 << 1,
    /// Repeat image in Y direction.
    FLD_IMAGE_REPEATY = 1 << 2,
    // Flips (inverses) image in Y direction when rendered.
    FLD_IMAGE_FLIPY = 1 << 3,
    // Image data has premultiplied alpha.
    FLD_IMAGE_PREMULTIPLIED = 1 << 4,
    // Image interpolation is Nearest instead Linear
    FLD_IMAGE_NEAREST = 1 << 5,
};

struct FldAllocator;
struct FldContext;
typedef uint64_t FldImage;
typedef uint64_t FldFont;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
    const char* text;
    size_t text_size;
} FldText;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FldText fld_text(const char* text, size_t text_size) {
    return (FldText){.text = text, .text_size = text_size};
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FldText fld_text_from_cstring(const char* text) {
#if defined(FLD_HAS_STRLEN)
    int len = strlen(text);
#else
    int len = 0;
    while (text[len]) {
        len++;
    }
#endif
    return fld_text(text, len);
}