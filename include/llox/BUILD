load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
  name = "libpolylox_headers",
  hdrs = [
    "ast-printer.h", "ast.h",
    "environment.h", "interpreter.h",
    "object.h", "parser.h", "scanner.h",
    "token.h", "util.h"
  ],
  strip_include_prefix = ".",
  include_prefix = "llox",
  visibility = ["//visibility:public"],
)
