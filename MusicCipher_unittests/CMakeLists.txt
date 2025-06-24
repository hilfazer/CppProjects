cmake_minimum_required(VERSION 3.16)

project(MusicCipherUnitTests LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MusicCipherUnitTests main.cpp)
add_test(NAME MusicCipherUnitTests COMMAND MusicCipherUnitTests)

if (DEFINED ENV{BOOST_INCLUDE_DIR})
    set(BOOST_INCLUDE_DIR $ENV{BOOST_INCLUDE_DIR})
else ()
    set(BOOST_INCLUDE_DIR "H:/Programming/vcpkg/installed/x64-windows/include/") # set by Qt Creator wizard
endif ()
if (BOOST_INCLUDE_DIR STREQUAL "")
    message("BOOST_INCLUDE_DIR is not set, assuming Boost can be found automatically in your system")
elseif (EXISTS ${BOOST_INCLUDE_DIR})
    include_directories(${BOOST_INCLUDE_DIR})
endif ()
