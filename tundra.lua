local native = require('tundra.native')

-----------------------------------------------------------------------------------------------------------------------

local mac_opts = {
    "-Wall", "-I.",
    "-DFLOWI_MAC",
    { "-DFLOWID_DEBUG", "-O0", "-g"; Config = "*-*-debug" },
    { "-DFLOWID_RELEASE", "-O3", "-g"; Config = "*-*-release" },
}

-----------------------------------------------------------------------------------------------------------------------

local macos = {
    Env = {
        CCOPTS =  {
            mac_opts,
        },

        CXXOPTS = {
            mac_opts,
        },

        ISPCOPTS = {
            "--target=sse2", "-I.",
        },
    },

    ReplaceEnv = {
        ISPC = "bin/macos/ispc$(PROGSUFFIX)"
    },
}

-----------------------------------------------------------------------------------------------------------------------

local gcc_opts = {
    "-I.",
    "-I$(OBJECTDIR)",
    "-Wall",
    "-fPIC",
    { "-DFLOWID_TEST", "-O2", "-g"; Config = "*-*-test" },
    { "-DFLOWID_RELEASE", "-O3", Config = "*-*-release" },
}

local gcc_env = {
    Env = {
        CCOPTS = {
            gcc_opts,
        },

        CXXOPTS = {
            gcc_opts,
        },

        ISPCOPTS = {
            "--target=sse2", "-I.",
        },
    },

    ReplaceEnv = {
        LD = "c++",
        ISPC = "bin/linux/ispc$(PROGSUFFIX)"
    },
}

-----------------------------------------------------------------------------------------------------------------------

local win64_opts = {
    "/EHsc", "/FS", "/MD", "/W3", "/I.", "/DUNICODE", "/D_UNICODE", "/DWIN32", "/D_CRT_SECURE_NO_WARNINGS",
    "\"/DOBJECT_DIR=$(OBJECTDIR:#)\"",
    { "/DFLOWI_DEBUG","/Od"; Config = "*-*-debug" },
    { "/DFLOWI_RELEASE", "/O2"; Config = "*-*-release" },
}

local win64 = {
    Env = {
        GENERATE_PDB = "1",
        CCOPTS = {
            win64_opts,
        },

        CXXOPTS = {
            win64_opts,
        },

        ISPCOPTS = {
            "--target=sse2",
        },

        OBJCCOM = "meh",
    },

    ReplaceEnv = {
        ISPC = "bin\\windows\\ispc$(PROGSUFFIX)"
    },
}

-----------------------------------------------------------------------------------------------------------------------

Build {
    Passes = {
        IspcGen = { Name = "IspcGen", BuildOrder = 1 },
    },

    Units = {
        "units.lua",
    },

    Configs = {
        Config { Name = "macos-clang", DefaultOnHost = "macosx", Inherit = macos, Tools = { "clang-osx", "ispc" } },
        Config { Name = "win64-msvc", DefaultOnHost = { "windows" }, Inherit = win64, Tools = { "msvc-vs2019", "ispc" } },
        Config { Name = "linux-gcc", DefaultOnHost = { "linux" }, Inherit = gcc_env, Tools = { "gcc", "ispc" } },
        Config { Name = "linux-clang", DefaultOnHost = { "linux" }, Inherit = gcc_env, Tools = { "clang", "ispc" } },
    },

    IdeGenerationHints = {
        Msvc = {
            -- Remap config names to MSVC platform names (affects things like header scanning & debugging)
            PlatformMappings = {
                ['win64-msvc'] = 'x64',
            },

            -- Remap variant names to MSVC friendly names
            VariantMappings = {
                ['release'] = 'Release',
                ['debug'] = 'Debug',
            },
        },

        MsvcSolutions = {
            ['Flowid.sln'] = { }
        },

    },

    Variants = { "debug", "release" },
    SubVariants = { "default", "test" },
}

-- vim: ts=4:sw=4:sts=4

