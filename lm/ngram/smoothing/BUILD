cc_library(
    name = "absolute_discounting",
    srcs = [
        "absolute_discounting.h",
        "absolute_discounting.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//tensorflow/Source/lm/ngram:ngram",
    ],
)

cc_test(
    name = "absolute_discounting_test",
    size = "small",
    srcs = ["absolute_discounting_test.cc"],
    deps = [
        ":absolute_discounting",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

cc_library(
    name = "add_one",
    srcs = [
        "add_one.h",
        "add_one.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//tensorflow/Source/lm/ngram:ngram",
    ],
)

cc_test(
    name = "add_one_test",
    size = "small",
    srcs = ["add_one_test.cc"],
    deps = [
        ":add_one",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

cc_library(
    name = "katz",
    srcs = [
        "katz.h",
        "katz.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//tensorflow/Source/lm/ngram:ngram",
    ],
)

cc_test(
    name = "katz_test",
    size = "small",
    srcs = ["katz_test.cc"],
    deps = [
        ":katz",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

cc_library(
    name = "kneser_ney",
    srcs = [
        "kneser_ney.h",
        "kneser_ney.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":absolute_discounting",
        "//tensorflow/Source/lm/ngram:ngram",
    ],
)

cc_test(
    name = "kneser_ney_test",
    size = "small",
    srcs = ["kneser_ney_test.cc"],
    deps = [
        ":kneser_ney",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

cc_library(
    name = "kneser_ney_mod",
    srcs = ["kneser_ney_mod.cc"],
    hdrs = ["kneser_ney_mod.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":kneser_ney",
        "//tensorflow/Source/lm/ngram:ngram",
    ],
)

cc_test(
    name = "kneser_ney_mod_test",
    size = "small",
    srcs = ["kneser_ney_mod_test.cc"],
    deps = [
        ":kneser_ney_mod",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)
