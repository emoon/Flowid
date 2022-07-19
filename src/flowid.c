#include "flowid_types.h"
#include "src/math_ispc.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FldColor fld_hsl_to_rgb(float h, float s, float l, float a) {
    FldColor output[16] = {0};
    fld_hsl_to_rgb_ispc((struct $anon0*)&output, h, s, l, a);
    return output[0];
}
