#include <flowid.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#ifdef FLD_INCLUDE_REF_CODE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on NanoVG: Original code by Mikko Mononen, additional code by Antti Käännänen.

static float clampf(float a, float mn, float mx) {
    return a < mn ? mn : (a > mx ? mx : a);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static float hue(float h, float m1, float m2) {
    if (h < 0)
        h += 1;
    if (h > 1)
        h -= 1;
    if (h < 1.0f / 6.0f)
        return m1 + (m2 - m1) * h * 6.0f;
    else if (h < 3.0f / 6.0f)
        return m2;
    else if (h < 4.0f / 6.0f)
        return m1 + (m2 - m1) * (2.0f / 3.0f - h) * 6.0f;
    return m1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FldColor fld_hsl_to_rgb_ref(float h, float s, float l, float a) {
    float m1, m2;
    FldColor col;
    h = fmodf(h, 1.0f);
    if (h < 0.0f) {
        h += 1.0f;
    }
    s = clampf(s, 0.0f, 1.0f);
    l = clampf(l, 0.0f, 1.0f);
    m2 = l <= 0.5f ? (l * (1 + s)) : (l + s - l * s);
    m1 = 2 * l - m2;
    col.r = clampf(hue(h + 1.0f / 3.0f, m1, m2), 0.0f, 1.0f);
    col.g = clampf(hue(h, m1, m2), 0.0f, 1.0f);
    col.b = clampf(hue(h - 1.0f / 3.0f, m1, m2), 0.0f, 1.0f);
    col.a = a;
    return col;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on https://raphlinus.github.io/graphics/curves/2019/12/23/flatten-quadbez.html

/*
eval(t) {
    const mt = 1 - t;
    const x = this.x0 * mt * mt + 2 * this.x1 * t * mt + this.x2 * t * t;
    const y = this.y0 * mt * mt + 2 * this.y1 * t * mt + this.y2 * t * t;
    return new Point(x, y);
}
    class QuadBez { constructor(x0, y0, x1, y1, x2, y2) { this.x0 = x0; this.y0 = y0; this.x1 = x1; this.y1 = y1;
this.x2 = x2; this.y2 = y2; }

map_to_basic() {
    const ddx = 2 * this.x1 - this.x0 - this.x2;
    const ddy = 2 * this.y1 - this.y0 - this.y2;
    const u0 = (this.x1 - this.x0) * ddx + (this.y1 - this.y0) * ddy;
    const u2 = (this.x2 - this.x1) * ddx + (this.y2 - this.y1) * ddy;
    const cross = (this.x2 - this.x0) * ddy - (this.y2 - this.y0) * ddx;
    const x0 = u0 / cross;
    const x2 = u2 / cross;
    // There's probably a more elegant formulation of this...
    const scale = Math.abs(cross) / (Math.hypot(ddx, ddy) * Math.abs(x2 - x0));
    return {x0: x0, x2: x2, scale: scale, cross: cross};
}

  approx_inv_myint(x) { const b = 0.39; return x * (1 - b + Math.sqrt(b * b + 0.25 * x * x)); }

  const params = this.map_to_basic();
    const a0 = approx_myint(params.x0);
    const a2 = approx_myint(params.x2);
    const count =  0.5 * Math.abs(a2 - a0) * Math.sqrt(params.scale / tol);
    const n = Math.ceil(count);
    const u0 = approx_inv_myint(a0);
    const u2 = approx_inv_myint(a2);
    let result = [0];
    for (let i = 1; i < n; i++) {
        const u = approx_inv_myint(a0 + ((a2 - a0) * i) / n);
        const t = (u - u0) / (u2 - u0);
        result.push(t);
    }
    result.push(1);
    return result;
*/

//#endif

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
