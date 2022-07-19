require "tundra.syntax.glob"
require "tundra.path"
require "tundra.util"
require "tundra.syntax.ispc"

local native = require('tundra.native')

-----------------------------------------------------------------------------------------------------------------------

local function get_c_src(dir)
    return Glob {
        Dir = dir,
        Extensions = { ".c", ".h" },
        Recursive = false,
}
end

-----------------------------------------------------------------------------------------------------------------------

StaticLibrary {
    Name = "flowid",

	Includes = {
        "include",
	},

    Sources = { 
        get_c_src("src"),
		ISPC { 
            Pass = "IspcGen",
            Source = "src/math.ispc"
        },
    },
}

-----------------------------------------------------------------------------------------------------------------------

Program {
    Name = "flowid_test",
	Includes = { "include" },
    Sources = { "src/test/tests.c" },
    Depends = { "flowid" },
}

Program {
    Name = "flowid_bench",
	Includes = { "include" },
    Sources = { "src/bench/bench.c" },
    Depends = { "flowid" },
}

-----------------------------------------------------------------------------------------------------------------------

Default "flowid_test"
Default "flowid_bench"

-- vim: ts=4:sw=4:sts=4

