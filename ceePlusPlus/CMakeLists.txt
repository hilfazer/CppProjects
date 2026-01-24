cmake_minimum_required(VERSION 3.16)

project(ceePlusPlus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ceePlusPlus main.cpp
    noisy_type.hpp shared_ptr.cpp)

include(GNUInstallDirs)
install(TARGETS ceePlusPlus
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
