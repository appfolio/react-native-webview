cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_STANDARD 14)

message( ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories(
    ${PROJECT_SOURCE_DIR}/../../../../node_modules/react-native/React
    ${PROJECT_SOURCE_DIR}/../../../../node_modules/react-native/React/Base
    ${PROJECT_SOURCE_DIR}/../../../../node_modules/react-native/ReactCommon/jsi
)

add_library(reactnativecommunity-webview-jsi SHARED
    # this seems necessary to use almost any JSI API - otherwise we get linker errors
    # seems wrong to compile a file that's already getting compiled as part of the app, but ¯\_(ツ)_/¯
    ${PROJECT_SOURCE_DIR}/../../../../node_modules/react-native/ReactCommon/jsi/jsi/jsi.cpp

    # source files
  RNCWebViewLifecycle.h
  RNCWebViewLifecycle.cpp
)

target_link_libraries(reactnativecommunity-webview-jsi
    # link with these libraries:
    android
    log
)

target_include_directories(reactnativecommunity-webview-jsi PUBLIC
    ${PROJECT_SOURCE_DIR}/../node_modules/react-native/ReactAndroid/src/main/jni/first-party/fb/include
)