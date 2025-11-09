load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
  name = "libpolylox",
  srcs = [
    "ast-printer.cpp", "interpreter.cpp",
    "parser.cpp", "scanner.cpp", "token.cpp"
  ],
  deps = [
    "//include/llox:libpolylox_headers",
  ],
  visibility = ["//visibility:public"],
)
