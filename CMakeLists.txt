cmake_minimum_required(VERSION 3.5)
project(LinuxProgrammingProject)

include_directories("${PROJECT_SOURCE_DIR}/include")
add_executable(console  ${PROJECT_SOURCE_DIR}/src/console.c)
add_executable(argument  ${PROJECT_SOURCE_DIR}/src/argument.c)