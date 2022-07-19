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

//#endif