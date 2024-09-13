cmake_minimum_required(VERSION 3.5)
project(NeuroCppProject)

set(SPEECHSDK_ROOT "/Users/skyliu/azurespeechservices_sdk/MicrosoftCognitiveServicesSpeech.xcframework/macos-arm64_x86_64")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the toolchain for vcpkg to find packages
set(CMAKE_TOOLCHAIN_FILE "/Users/skyliu/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Include directories for PortAudio, CPR, and Speech SDK
include_directories(/usr/local/include)
include_directories(/Users/skyliu/vcpkg/packages/portaudio_arm64-osx/include)
include_directories(/Users/skyliu/vcpkg/packages/cpr_arm64-osx/include)
include_directories(${SPEECHSDK_ROOT}/MicrosoftCognitiveServicesSpeech.framework/Headers)

# Link directories (ensure proper use of framework search paths)
link_directories(/usr/local/lib)
link_directories(/Users/skyliu/vcpkg/installed/arm64-osx/lib)


file(GLOB SOURCE_FILES "*.cpp")

add_executable(neuro main.cpp ${SOURCE_FILES} process/llm.cpp input/stt.cpp )

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
