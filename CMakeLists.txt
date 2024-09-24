cmake_minimum_required(VERSION 3.19.0...3.27.0)
project(RayTracingEngine LANGUAGES CXX)

# Set to C++11
set(CMAKE_CXX_STANDARD          11 )
set(CMAKE_CXX_STANDARD_REQUIRED ON )
set(CMAKE_CXX_EXTENSIONS        OFF)

# Source
include_directories(source)

# Specific compiler flags below. We're not going to add options for all possible compilers, but if
# you're new to CMake (like we are), the following may be a helpful example if you're using a
# different compiler or want to set different compiler options.

message(STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID}    )
message(STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG}    )

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # /wd #### - Disable warning
    # /we #### - Treat warning as error
    add_compile_options("/W4")                     # Enable level-4 warnings
    add_compile_options("/we 4265")                # Class has virtual functions, but its non-trivial destructor is not virtual
    add_compile_options("/we 5204")                # Class has virtual functions, but its trivial destructor is not virtual
    add_compile_options("/wd 4100")                # Unreferenced formal parameter
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wnon-virtual-dtor)        # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder)                 # Data member will be initialized after [other] data member
    add_compile_options(-Wmaybe-uninitialized)     # Variable improperly initialized
    add_compile_options(-Wunused-variable)         # Variable is defined but unused
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wnon-virtual-dtor)        # Class has virtual functions, but its destructor is not virtual
    add_compile_options(-Wreorder)                 # Data member will be initialized after [other] data member
    add_compile_options(-Wsometimes-uninitialized) # Variable improperly initialized
    add_compile_options(-Wunused-variable)         # Variable is defined but unused
endif()

# Executables
add_executable(Main source/Main.cpp)