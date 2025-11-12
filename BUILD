load("@aspect_rules_lint//format:defs.bzl", "format_multirun")
load("@buildifier_prebuilt//:rules.bzl", "buildifier")

format_multirun(
    name = "format",
    python = "@aspect_rules_lint//format:ruff",
)

buildifier(
    name = "buildifier",
    lint_mode = "warn",
    mode = "fix",
    verbose = True,
)
