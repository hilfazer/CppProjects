cmake_minimum_required(VERSION 3.16)

project(BoostTestExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BoostTestExample main.cpp)

include(GNUInstallDirs)
install(TARGETS BoostTestExample
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
