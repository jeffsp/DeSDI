cmake_minimum_required(VERSION 3.28)
project(desdi-parser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WARN_DEPRECATED OFF)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}") # also for C if needed by dependencies
endif()

set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/.deps CACHE PATH "Directory for downloaded dependencies")
include(FetchContent)

FetchContent_Declare(HighFive
  GIT_REPOSITORY https://github.com/BlueBrain/HighFive.git
  GIT_TAG        v2.10.0
)
FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)
FetchContent_Declare(CLI11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG        v2.4.2
)
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)

set(HIGHFIVE_USE_BOOST OFF CACHE BOOL "" FORCE)
set(HIGHFIVE_UNIT_TESTS OFF CACHE STRING "" FORCE)
set(HIGHFIVE_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(HighFive nlohmann_json CLI11 Catch2)

find_package(HDF5 REQUIRED COMPONENTS C)
find_package(OpenSSL REQUIRED)

add_library(desdi_lib INTERFACE)
target_include_directories(desdi_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(desdi_lib INTERFACE
    HighFive
    nlohmann_json::nlohmann_json
    OpenSSL::Crypto
    HDF5::HDF5
)

add_executable(desdi-ingest src/main.cpp)
target_link_libraries(desdi-ingest PRIVATE desdi_lib CLI11::CLI11)

enable_testing()
add_subdirectory(tests)
