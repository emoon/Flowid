//#include "include/flowid_types.h"

/*
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
*/

void dummy_function() {
}
