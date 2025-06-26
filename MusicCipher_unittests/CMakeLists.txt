cmake_minimum_required(VERSION 3.16)

project(MusicCipherUnitTests LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MusicCipherUnitTests main.cpp
    ../MusicCipher/Octave.cpp
    Octave_unittests.cpp
    OctaveKeysParser_unittests.cpp
    ../MusicCipher/OctaveKeysParser.cpp)
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

include_directories("../MusicCipher") # tested project

find_package(Qt5 REQUIRED COMPONENTS Widgets )

target_link_libraries(MusicCipherUnitTests PRIVATE Qt5::Widgets)
