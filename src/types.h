#pragma once

#include <stdbool.h>
#include <stdint.h>

#define FLD_RESTRICT __restrict

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef float f32;
typedef double f64;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef FLT_MAX
#define FLT_MAX 3.402823466e+38F
#endif

#if defined(__GNUC__)
#define FLD_INLINE static inline
#elif defined(_MSC_VER)
#define FLD_INLINE __forceinline static
#else
#error "Unsupported compiler"
#endif

#if defined(__GNUC__) || defined(__clang__)
#include <stdalign.h>
#define FLD_LIKELY(x) __builtin_expect((x), 1)
#define FLD_UNLIKELY(x) __builtin_expect((x), 0)
#define FLD_ALIGNOF(_type) alignof(_type)
#else
#define FLD_ALIGNOF(_type) __alignof(_type)
#define FLD_LIKELY(x) (x)
#define FLD_UNLIKELY(x) (x)
#endif

#define FLD_MIN(a, b) ((a) < (b)) ? (a) : (b)
#define FLD_MAX(a, b) ((a) > (b)) ? (a) : (b)
#define FLD_UNUSED(a) (void)a

#if defined(__GNUC__)
#if defined(__clang__)
#define FL_ASSUME(cond) __builtin_assume(cond)
#else
#define FL_ASSUME(cond)              \
    do {                             \
        if (!(cond))                 \
            __builtin_unreachable(); \
    } while (0)
#endif
#else
#define FL_ASSUME(cond) \
    while (0) {         \
    }
#endif
