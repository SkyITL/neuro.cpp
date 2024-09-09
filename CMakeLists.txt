# CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(NeuroCppProject)

set(SPEECHSDK_ROOT "/Users/skyliu/azurespeechservices_sdk/MicrosoftCognitiveServicesSpeech.xcframework/macos-arm64_x86_64")

include_directories(${SPEECHSDK_ROOT}/MicrosoftCognitiveServicesSpeech.framework/Headers)

link_directories(${SPEECHSDK_ROOT}/MicrosoftCognitiveServicesSpeech.framework)

file(GLOB SOURCE_FILES "*.cpp")

add_executable(neuro main.cpp ${SOURCE_FILES})

target_link_libraries(neuro MicrosoftCognitiveServicesSpeech)