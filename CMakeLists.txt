cmake_minimum_required(VERSION 2.8.9)

project(TCPServerCalculator)
set (TcpServerCalculator_VERSION_MAJOR 1)
set (TcpServerCalculator_VERSION_MINOR 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(server ${SOURCES})
add_executable(client ${SOURCES})

set_target_properties(server PROPERTIES COMPILE_FLAGS "-DSERVER")
set_target_properties(client PROPERTIES COMPILE_FLAGS "-DCLIENT")


#add_library(tcpTransmission SHARED ${SOURCES})

#install(TARGETS tcpTransmission DESTINATION /usr/lib)
