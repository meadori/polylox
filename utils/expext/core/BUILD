load("@buildifier_prebuilt//:rules.bzl", "buildifier")
load("@rules_python//python:defs.bzl", "py_binary", "py_library", "py_test")

py_library(
    name = "core",
    srcs = [
        "__init__.py",
        "collector.py",
        "matcher.py",
        "runner.py",
    ],
    visibility = ["//visibility:public"],
)
