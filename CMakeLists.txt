cmake_minimum_required(VERSION 3.15)
project(NeuroCppProject)

set(SPEECHSDK_ROOT "/Users/skyliu/azurespeechservices_sdk/MicrosoftCognitiveServicesSpeech.xcframework/macos-arm64_x86_64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the toolchain for vcpkg to find packages
set(CMAKE_TOOLCHAIN_FILE "/Users/skyliu/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# include directories of tensorflow
include_directories(
    /Users/skyliu/tensorflow  # TensorFlow source directory
    /private/var/tmp/_bazel_skyliu/5470dcdb7edc5a07b167a774dce0c331/execroot/org_tensorflow/bazel-out/darwin_arm64-opt/bin/tensorflow
    
)
include_directories(/private/var/tmp/_bazel_skyliu/5470dcdb7edc5a07b167a774dce0c331/execroot/org_tensorflow/bazel-out/darwin_arm64-opt/bin/external/eigen_archive)


include_directories(/Users/skyliu/abseil-cpp/absl)

# Include directories for PortAudio, CPR, and Speech SDK
include_directories(/usr/local/include)

include_directories(/Users/skyliu/vcpkg/packages/portaudio_arm64-osx/include)
include_directories(/Users/skyliu/vcpkg/packages/cpr_arm64-osx/include)
include_directories(${SPEECHSDK_ROOT}/MicrosoftCognitiveServicesSpeech.framework/Headers)

# Link directories (ensure proper use of framework search paths)
link_directories(/usr/local/lib)
link_directories(/Users/skyliu/vcpkg/installed/arm64-osx/lib)
link_directories(/private/var/tmp/_bazel_skyliu/5470dcdb7edc5a07b167a774dce0c331/execroot/org_tensorflow/bazel-out/darwin_arm64-opt/bin/tensorflow)
link_directories(/Users/skyliu/abseil-cpp/absl)

file(GLOB SOURCE_FILES "*.cpp")

add_executable(neuro main.cpp ${SOURCE_FILES} process/llm.cpp input/stt.cpp output/tts.cpp inference.cpp)

find_library(METAL_FRAMEWORK Metal)
find_library(MPS_FRAMEWORK MetalPerformanceShaders)

find_package(absl REQUIRED)
target_link_libraries(neuro PRIVATE absl::status absl::strings absl::base)


target_link_libraries(neuro PRIVATE ${METAL_FRAMEWORK} ${MPS_FRAMEWORK})

# Ensure we're building for arm64 architecture
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Link the Speech SDK framework properly using the -F flag
target_link_libraries(neuro PRIVATE "-framework MicrosoftCognitiveServicesSpeech")
target_link_libraries(neuro PRIVATE "-framework CoreAudio" "-framework CoreFoundation" "-framework AudioUnit" "-framework AudioToolbox" "-framework CoreServices")
target_link_options(neuro PRIVATE -F${SPEECHSDK_ROOT})  # Correctly specify the framework search path

set_target_properties(neuro PROPERTIES
        INSTALL_RPATH "${SPEECHSDK_ROOT}"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH_USE_LINK_PATH TRUE
)

# Manually add linking options if required
target_link_libraries(neuro PRIVATE /Users/skyliu/vcpkg/installed/arm64-osx/lib/libportaudio.a)
target_link_libraries(neuro PRIVATE /Users/skyliu/vcpkg/installed/arm64-osx/lib/libcpr.a)
target_link_libraries(neuro PRIVATE tensorflow_cc tensorflow_framework)
target_link_libraries(neuro PRIVATE absl_status absl_strings absl_base)