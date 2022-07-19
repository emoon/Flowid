#include "include/flowid_types.h"

static float hue(varying float h, varying float m1, varying float m2) {
    h = select(h < 0.0f, h + 1, h);
    h = select(h > 1.0f, h - 1, h);

    varying float t0 =  m1 + (m2 - m1) * h * 6.0;
    varying float t1 =  m1 + (m2 - m1) * (2.0f/3.0f - h) * 6.0;
    varying float r = select(h < 1.0f/6.0f, t0, select(h < 3.0f/6.0f, m2, select(h < 4.0f/6.0f, t1, m1)));
    return r;
}

static uniform float mod(uniform float x, uniform float y) {
    return x - y * floor(x / y);
}

export void fld_hsl_to_rgb_ispc(uniform FldColor output[], uniform float h, uniform float s, uniform float l, uniform float a) {
    uniform float temp[TARGET_WIDTH * programCount];
    h = mod(h, 1.0f);
	h = (h < 0.0f) ? h += 1.0f : h;
	s = clamp(s, 0.0f, 1.0f);
	l = clamp(l, 0.0f, 1.0f);
    uniform float t0 = (l * (1 + s));
    uniform float t1 = (l + s - l * s);
	uniform float m2 = l <= 0.5f ? t0 : t1;
	uniform float m1 = 2 * l - m2;
	varying float r = clamp(hue(h + 1.0f / 3.0f, m1, m2), 0.0f, 1.0f);
	varying float g = clamp(hue(h, m1, m2), 0.0f, 1.0f);
	varying float b = clamp(hue(h - 1.0f / 3.0f, m1, m2), 0.0f, 1.0f);
    varying float va = a;
    soa_to_aos4(r, g, b, va, temp);
    output[0].r = temp[0];
    output[0].g = temp[1];
    output[0].b = temp[2];
    output[0].a = a;
}

export void eval_quadratic_bezier(uniform float out_x[], uniform float out_y[], const uniform float in_x0[],
                                  const uniform float in_x1[], const uniform float in_x2[], const uniform float in_y0[],
                                  const uniform float in_y1[], const uniform float in_y2[], const uniform float time[],
                                  const uniform uint64 count) {
    assume(count % 8 == 0);
    foreach (i = 0 ... count) {
        const varying float x0 = in_x0[i];
        const varying float x1 = in_x1[i];
        const varying float x2 = in_x2[i];
        const varying float y0 = in_y0[i];
        const varying float y1 = in_y1[i];
        const varying float y2 = in_y2[i];
        const varying float t = time[i];
        const varying float mt = 1.0f - t;

        varying float x = x0 * mt * mt + 2 * x1 * t * mt + x2 * t * t;
        varying float y = y0 * mt * mt + 2 * y1 * t * mt + y2 * t * t;

        out_x[i] = x;
        out_y[i] = y;
    }
}

varying float approx_inv_myint(varying float x) {
    const float b = 0.39;
    return x * (1 - b + sqrt(b * b + 0.25 * x * x));
}

varying float hypot(varying float x, varying float y) {
    return sqrt(x * x + y * y);
}

export void calc_segment_div_inter(uniform int32 counts[], uniform float a0_out[], uniform float a2_out[],
                                   uniform float u0_out[], uniform float u2_out[], const uniform float in_x0[],
                                   const uniform float in_x1[], const uniform float in_x2[],
                                   const uniform float in_y0[], const uniform float in_y1[],
                                   const uniform float in_y2[], const uniform uint64 curve_count) {
    const uniform float tol = 0.5f;

    foreach (i = 0 ... curve_count) {
        float x0 = in_x0[i];
        float x1 = in_x1[i];
        float x2 = in_x2[i];
        float y0 = in_y0[i];
        float y1 = in_y1[i];
        float y2 = in_y2[i];

        const float ddx = 2 * x1 - x0 - x2;
        const float ddy = 2 * y1 - y0 - y2;
        const float u0 = (x1 - x0) * ddx + (y1 - y0) * ddy;
        const float u2 = (x2 - x1) * ddx + (y2 - y1) * ddy;
        const float cross = (x2 - x0) * ddy - (y2 - y0) * ddx;
        const float a0 = approx_inv_myint(u0 / cross);
        const float a2 = approx_inv_myint(u2 / cross);
        const float scale = abs(cross) / (hypot(ddx, ddy) * abs(x2 - x0));
        const float count = 0.5f * abs(a2 - a0) * sqrt(scale / tol);
        const int32 n = (int32)ceil(count);

        counts[i] = n;
        a0_out[i] = a0;
        a2_out[i] = a2;
        u0_out[i] = u0;
        u2_out[i] = u2;
    }
}

export void calc_segment_div(uniform float times[], const uniform float a0_in[], const uniform float a2_in[],
                             const uniform float u0_in[], const uniform float u2_in[], const uniform uint64 seg_count) {
    const uniform float tol = 0.5f;
    const uniform float inv_n = 1.0f / seg_count;

    foreach (i = 0 ... seg_count) {
        const varying float a0 = a0_in[i];
        const varying float a2 = a2_in[i];
        const varying float u0 = u0_in[i];
        const varying float u2 = u2_in[i];

        const float u = approx_inv_myint(a0 + ((a2 - a0) * i) * inv_n);
        const float t = (u - u0) / (u2 - u0);

        times[i] = t;
    }
}