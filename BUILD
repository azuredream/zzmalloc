#bazel test --test_output=all //:alloc_test --verbose_failures --cxxopt=-std=c++17
#bazel build alloc --cxxopt=-std=c++17
#bazel build main  

#./bazel-bin/main

cc_binary(
    name = "main",
    srcs = ["src/main.cc"],
    deps = ["//:alloc"],
    copts = ["-std=c++17", "-ggdb"]
)

cc_library(
    name = "alloc",
    srcs = glob(
        ["src/*.cc"],
        ["src/*.h"]
    ),
    hdrs = glob([
        "include/**/*.h"
    ]),
    copts = ["-Iinclude/", "-std=c++17", "-ggdb"],
    linkopts = ["-pthread"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "alloc_test",
    size = "small",
    srcs = glob(
        ["test/*.cc"],
        ["test/*.h"]
    ),
    copts = ["-std=c++17", "-ggdb"],
    deps = ["@com_google_googletest//:gtest_main",
            "//:alloc"],
)