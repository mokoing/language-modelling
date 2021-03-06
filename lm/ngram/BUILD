package(default_visibility = ["//visibility:public"])

load(
    "//tensorflow/core:platform/default/build_config.bzl",
    "tf_proto_library",
)

cc_library(
    name = "ngram",
    srcs = ["ngram.cc"],
    hdrs = ["ngram.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":count_trie",
        ":ngram_pb_cc",
        ":prob_trie",
        "//tensorflow/Source/lm:lm",
    ],
)

cc_test(
    name = "ngram_test",
    size = "small",
    srcs = ["ngram_test.cc"],
    deps = [
        ":ngram",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

tf_proto_library(
    name = "ngram_pb",
    srcs = [
        "ngram.proto",
        "prob_trie.proto",
    ],
    cc_api_version = 2,
    visibility = ["//visibility:public"],
)

cc_library(
    name = "prob_trie",
    srcs = [
        "prob_trie.h",
        "prob_trie.cc",
    ],
    deps = [":ngram_pb_cc"],
)

cc_test(
    name = "prob_trie_test",
    size = "small",
    srcs = ["prob_trie_test.cc"],
    deps = [
        ":prob_trie",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

cc_library(
    name = "count_trie",
    srcs = [
        "count_trie.h",
        "count_trie.cc",
    ],
    deps = [
        "//tensorflow/Source/lm:vocab",
        "//tensorflow/Source/util:reader",
    ],
)

cc_test(
    name = "count_trie_test",
    size = "small",
    srcs = ["count_trie_test.cc"],
    deps = [
        ":count_trie",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ],
)

cc_binary(
    name = "train",
    srcs = ["train.cc"],
    deps = [
        ":ngram",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:lib",
        "//tensorflow/core:framework_internal",
        "//tensorflow/core:framework_headers_lib",
        "//tensorflow/Source/lm/ngram/smoothing:absolute_discounting",
        "//tensorflow/Source/lm/ngram/smoothing:add_one",
        "//tensorflow/Source/lm/ngram/smoothing:katz",
        "//tensorflow/Source/lm/ngram/smoothing:kneser_ney",
        "//tensorflow/Source/lm/ngram/smoothing:kneser_ney_mod",
    ],
)

cc_library(
    name = "load",
    srcs = ["load.cc"],
    hdrs = ["load.h"],
    deps = [
        ":ngram",
        ":prob_trie",
        "//tensorflow/Source/lm/ngram/smoothing:absolute_discounting",
        "//tensorflow/Source/lm/ngram/smoothing:add_one",
        "//tensorflow/Source/lm/ngram/smoothing:katz",
        "//tensorflow/Source/lm/ngram/smoothing:kneser_ney",
        "//tensorflow/Source/lm/ngram/smoothing:kneser_ney_mod",
        "//tensorflow/Source/lm:vocab",
    ],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "load_test",
    srcs = ["load_test.cc"],
    deps = [
        ":load",
        "//tensorflow/core:test",
        "//tensorflow/core:test_main",
    ]
)
