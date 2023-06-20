cmake_minimum_required(VERSION 3.0.0)
project(docker VERSION 0.1.0)

include_directories("include")
include_directories("cgroup")

aux_source_directory(. SRC_LIST)
add_subdirectory(cgroup)
add_executable(docker ${SRC_LIST})
add_definitions(-w)
target_link_libraries(docker cgroup stdc++fs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)