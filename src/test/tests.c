#include <math.h>
#include "flowid.h"
#include "flowid_types.h"
#include "utest.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
UTEST(color_convert, hsl_to_rgb_compare_with_ref) {
    FldColor res0 = fld_hsl_to_rgb(1.0f, 0.5f, 0.2f, 0.5f);
    FldColor res1 = fld_hsl_to_rgb_ref(1.0f, 0.5f, 0.2f, 0.5f);

    ASSERT_NEAR(res1.r, res0.r, 0.001f);
    ASSERT_NEAR(res1.g, res0.g, 0.001f);
    ASSERT_NEAR(res1.b, res0.b, 0.001f);
    ASSERT_NEAR(res1.a, res0.a, 0.001f);
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UTEST_MAIN();