#pragma once

#include "../include/flowid.h"
#include "linear_allocator.h"
#include "command_buffer.h"

#define SOA_ARRAY_WIDTH 8

/// Quadratic Bezier curves 
typedef struct {
    float x0[SOA_ARRAY_WIDTH], y0[SOA_ARRAY_WIDTH];
    float x1[SOA_ARRAY_WIDTH], y1[SOA_ARRAY_WIDTH];
    float x2[SOA_ARRAY_WIDTH], y2[SOA_ARRAY_WIDTH];
} SoAQuadraticCurve;

/// Cubic Bezier curves 
typedef struct {
    float x1[SOA_ARRAY_WIDTH], y1[SOA_ARRAY_WIDTH];
    float x2[SOA_ARRAY_WIDTH], y2[SOA_ARRAY_WIDTH];
    float x3[SOA_ARRAY_WIDTH], y3[SOA_ARRAY_WIDTH];
    float x4[SOA_ARRAY_WIDTH], y4[SOA_ARRAY_WIDTH];
} SoACubicBezierCurve;

typedef enum Commands {
    FLD_PATH_BEGIN,
    FLD_PATH_CLOSE,
    FLD_PATH_MOVE_TO,
    FLD_PATH_LINE_TO,
    FLD_PATH_RECT,
    FLD_PATH_ROUND_RECT,
    FLD_PATH_ARC_TO,
    FLD_PATH_ELLIPSE,
    FLD_PATH_QUDRATIC_CURVE_TO,
    FLD_PATH_CUBIC_CURVE_TO,
    FLD_PATH_STROKE,
    FLD_PATH_FILL,
} Commands;

typedef struct MoveToCommand {
    float x, y;
} MoveToCommand;

typedef struct LineToCommand {
    float x, y;
}  LineToCommand;

typedef struct RectCommand {
    float x, y, width, height;
} RectCommand;

typedef struct QadraticCurveToCommand {
    float x1, y1, x2, y2;
} QadraticCurveToCommand;

typedef struct CubicCurveToCommand {
    float x1, y1, x2, y2, x3, y3;
} CubicCurveToCommand;

typedef struct FldContext {
    FldAllocator allocator;
    CommandBuffer commands;
} FldContext;

#define Allocator_alloc(allocator, type) (type*)(allocator->alloc)(allocator->user_data, sizeof(type), FLD_ALIGNOF(type)) 
#define Allocator_realloc_array(allocator, type, old_ptr, old_size, new_size) \
    (type*)(allocator->realloc)(allocator->user_data, old_ptr, old_size, new_size * sizeof(type), FLD_ALIGNOF(type))  

#define Allocator_alloc_array(allocator, len, type) (type*)(allocator->alloc)(allocator->user_data, len * sizeof(type), FLD_ALIGNOF(type)) 
#define Allocator_free(allocator, ptr) (allocator->free)(allocator->user_data, ptr) 