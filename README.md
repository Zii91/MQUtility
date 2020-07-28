bazel test -c opt --macos_sdk_version=10.15 --test_output=errors test:tests
bazel build -c opt --macos_sdk_version=10.15 src/main:MQUtility
bazel run -c opt --macos_sdk_version=10.15 src/main:MQUtility


bazel test --test_output=errors test:tests
