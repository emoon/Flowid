#include <flowid.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//#ifdef FLD_INCLUDE_REF_CODE

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Based on NanoVG: Original code by Mikko Mononen, additional code by Antti Käännänen.

#if 0

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

#endif

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
