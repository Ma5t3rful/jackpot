cmake_minimum_required(VERSION 3.20)
project(shufflerbot)
set(CMAKE_CXX_STANDARD 26)
include(FetchContent)
set(FETCHCONTENT_QUIET false)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 1.12.0
                         GIT_PROGRESS true) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)
file(GLOB shuffler_src src/*.cpp)
add_executable(shufflerbot)
target_sources(shufflerbot PRIVATE ${shuffler_src} src/shuffler.cpp)
target_compile_options(shufflerbot PRIVATE -fhardened)
target_link_libraries(shufflerbot PRIVATE cpr::cpr nlohmann_json::nlohmann_json)