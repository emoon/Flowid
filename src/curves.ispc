#include "flowid_private.h"

struct SegmentCountOutput {
    float a0[SOA_ARRAY_WIDTH];
    float a2[SOA_ARRAY_WIDTH];
    float u0[SOA_ARRAY_WIDTH];
    float u2[SOA_ARRAY_WIDTH];
    int32 count[SOA_ARRAY_WIDTH];
};

varying float approx_inv_myint(varying float x) {
    const float b = 0.39;
    return x * (1 - b + sqrt(b * b + 0.25 * x * x));
}

varying float hypot(varying float x, varying float y) {
    return sqrt(x * x + y * y);
}

/// Calculates the segment count for each Quadratic Bezier curve given a tolerance value.
/// This code is based on <insert link>
export void QuadraticBezier_calc_segment_count(uniform SegmentCountOutput output[],
                                               const uniform SoAQuadraticCurve curves[],
                                               const uniform float tolerance,
                                               const uniform uint64 curve_count) {
    const uniform float inv_tolerance = rcp(tolerance);

    for (uniform uint64 i = 0; i < curve_count; i += programCount) {
        #pragma unroll
        for (uniform int j = 0; j < SOA_ARRAY_WIDTH; j += programCount) {
            varying int index = j + programIndex;

            float x0 = curves[i].x0[index];
            float y0 = curves[i].y0[index];
            float x1 = curves[i].x1[index];
            float y1 = curves[i].y1[index];
            float x2 = curves[i].x2[index];
            float y2 = curves[i].y2[index];

            const float ddx = 2 * x1 - x0 - x2;
            const float ddy = 2 * y1 - y0 - y2;
            const float u0 = (x1 - x0) * ddx + (y1 - y0) * ddy;
            const float u2 = (x2 - x1) * ddx + (y2 - y1) * ddy;
            const float cross = (x2 - x0) * ddy - (y2 - y0) * ddx;
            const float inv_cross = rcp(cross);
            const float a0 = approx_inv_myint(u0 * inv_cross);
            const float a2 = approx_inv_myint(u2 * inv_cross);
            const float scale = abs(cross) / (hypot(ddx, ddy) * abs(x2 - x0));
            const float count = 0.5f * abs(a2 - a0) * sqrt(scale * inv_tolerance);
            const int32 n = (int32)ceil(count);

            output[i].count[index] = n;
            output[i].a0[index] = a0;
            output[i].a2[index] = a2;
            output[i].u0[index] = a0;
            output[i].u2[index] = a2;
        }
    }
}

/// Samples the curve along the calculated 
export void QuadraticBezier_sample_curves(uniform float output[],
                                          const uniform SegmentCountOutput segment_info[],
                                          const uniform SoAQuadraticCurve curves[],
                                          const uniform uint64 curve_count) {
    uniform uint64 output_count = 0;

    for (uniform uint64 c = 0; c < curve_count; ++c) {
        uniform uint64 ai = c / programCount;
        uniform uint64 as = c % programCount;

        const uniform float a0 = segment_info[ai].a0[as];
        const uniform float a2 = segment_info[ai].a2[as];
        const uniform float u0 = segment_info[ai].u0[as];
        const uniform float u2 = segment_info[ai].u2[as];

        const uniform float x0 = curves[ai].x0[as];
        const uniform float y0 = curves[ai].y0[as];
        const uniform float x1 = curves[ai].x1[as];
        const uniform float y1 = curves[ai].y1[as];
        const uniform float x2 = curves[ai].x2[as];
        const uniform float y2 = curves[ai].y2[as];

        const uniform uint64 segment_count = segment_info[ai].count[as];
        const uniform float inv_n = 1.0f / segment_count;

        for (uniform uint64 s = 0; s < segment_count; s += programCount) {
            float temp = s;
            // Calculate time on the curve
            const float u = approx_inv_myint(a0 + ((a2 - a0) * temp) * inv_n);
            // TODO: u2 - u0 is constant so we should precalc outside loop
            const float t = (u - u0) / (u2 - u0);
            const float mt = 1.0f - t;
            const float mt2 = mt * mt;
            const float t2 = t * t;

            // Calculate the point in 2d space
            float x = x0 * mt2 + 2 * x1 * t * mt + x2 * t2;
            float y = y0 * mt2 + 2 * y1 * t * mt + y2 * t2;

            soa_to_aos2(x, y, &output[output_count * 2]);
            output_count++;
        }
    } 
}

/*
    /// Computes the number of quadratic bézier segments required to approximate this cubic curve
    /// given a tolerance threshold.
    ///
    /// Derived by Raph Levien from section 10.6 of Sedeberg's CAGD notes
    /// <https://scholarsarchive.byu.edu/cgi/viewcontent.cgi?article=1000&context=facpub#section.10.6>
    /// and the error metric from the caffein owl blog post <http://caffeineowl.com/graphics/2d/vectorial/cubic2quad01.html>
    pub fn num_quadratics(&self, tolerance: S) -> u32 {
        self.num_quadratics_impl(tolerance).to_u32().unwrap_or(1)
    }

    fn num_quadratics_impl(&self, tolerance: S) -> S {
        debug_assert!(tolerance > S::ZERO);

        let x = self.from.x - S::THREE * self.ctrl1.x + S::THREE * self.ctrl2.x - self.to.x;
        let y = self.from.y - S::THREE * self.ctrl1.y + S::THREE * self.ctrl2.y - self.to.y;

        let err = x * x + y * y;

        (err / (S::value(432.0) * tolerance * tolerance)).powf(S::ONE / S::SIX).ceil().max(S::ONE)
    }
*/


/*
/// Convert cubic bezier curves to quadratic
void convert_cubic_to_quadratic() {
    foreach (i = 0 ... cubic_count) {
        let x = self.from.x - S::THREE * self.ctrl1.x + S::THREE * self.ctrl2.x - self.to.x;
        let y = self.from.y - S::THREE * self.ctrl1.y + S::THREE * self.ctrl2.y - self.to.y;
    }
}
*/

