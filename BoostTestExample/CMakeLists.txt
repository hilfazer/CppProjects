cmake_minimum_required(VERSION 3.16)

project(BoostTestExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BoostTestExample main.cpp
    suiteTwo.cpp)

include(GNUInstallDirs)
install(TARGETS BoostTestExample
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if (DEFINED ENV{BOOST_INCLUDE_DIR})
    set(BOOST_INCLUDE_DIR $ENV{BOOST_INCLUDE_DIR})
else ()
    set(BOOST_INCLUDE_DIR "G:/Programming/vcpkg/installed/x64-windows/include/") # set by Qt Creator wizard
endif ()
if (BOOST_INCLUDE_DIR STREQUAL "")
    message("BOOST_INCLUDE_DIR is not set, assuming Boost can be found automatically in your system")
elseif (EXISTS ${BOOST_INCLUDE_DIR})
    include_directories(${BOOST_INCLUDE_DIR})
endif ()
