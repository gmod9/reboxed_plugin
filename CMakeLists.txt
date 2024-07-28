cmake_minimum_required(VERSION 3.14)

project(
  toybox_plugin
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
option(SAFETYHOOK_FETCH_ZYDIS "" ON)
# Safetyhook
FetchContent_Declare(
    safetyhook
    GIT_REPOSITORY "https://github.com/cursey/safetyhook.git"
    GIT_TAG "origin/main"
)
FetchContent_MakeAvailable(safetyhook)

add_compile_definitions(toybox_plugin UNICODE)
add_library(toybox_plugin SHARED src/main.cpp src/plugin.cpp src/utils.cpp src/vpk.cpp)
target_link_libraries(toybox_plugin PRIVATE safetyhook)