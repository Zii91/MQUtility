# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "MQUtilityLib",
  srcs = glob(["**/*.cpp"]),
  hdrs = glob(["**/*.h"]),
  deps = [
      "@googletest//:gtest_main"
  ],
  visibility = ["//visibility:public"],
)
