cmake_minimum_required(VERSION 3.10)

project(license)

aux_source_directory(. SRCS)

set(OPENSSL_DIR ${CMAKE_SOURCE_DIR}/3rdparty/openssl)
set(CMAKE_C_FLAGS "-Wall -g")

include_directories(${OPENSSL_DIR}/include)
link_directories(${OPENSSL_DIR})

#add_definitions(-DTEST_CODEC=1)

add_executable(decrypt codec.c misc.c)
target_link_libraries(decrypt crypto)
set_target_properties(decrypt PROPERTIES COMPILE_DEFINITIONS "TEST_CODEC=1")

add_executable(${PROJECT_NAME} license.c misc.c io.c codec.c)
target_link_libraries(${PROJECT_NAME} crypto)
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS "DISABLE_HIVE=1")

add_executable(test test.c)
