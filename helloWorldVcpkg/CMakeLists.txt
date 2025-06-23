cmake_minimum_required(VERSION 3.16)

project(helloWorldVcpkg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(helloWorldVcpkg main.cpp
    vcpkg.json)

find_package(fmt REQUIRED)
target_link_libraries(helloWorldVcpkg PRIVATE fmt::fmt)

find_package(raylib CONFIG REQUIRED)
target_link_libraries(helloWorldVcpkg PRIVATE raylib)

include(GNUInstallDirs)
install(TARGETS helloWorldVcpkg
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
