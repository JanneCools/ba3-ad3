cmake_minimum_required(VERSION 3.23.2)
project(AD3-project-2022-2023 C)

set(CMAKE_C_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -g -O3)

include_directories(include)

add_library(arraytrie SHARED include/arraytrie.h src/arraytrie.c)
add_library(ternarytrie SHARED include/ternarytrie.h src/ternarytrie.c)
add_library(customtrie SHARED include/customtrie.h src/customtrie.c)

add_executable(test_array test/test_arraytrie.c)
target_link_libraries(test_array PUBLIC arraytrie)

add_executable(test_ternary test/test_ternarytrie.c)
target_link_libraries(test_ternary PUBLIC ternarytrie)

add_executable(test_custom test/test_customtrie.c)
target_link_libraries(test_custom PUBLIC customtrie)

