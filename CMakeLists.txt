cmake_minimum_required(VERSION 3.25.0)
project(flex_bison VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

add_subdirectory(wc)
add_subdirectory(calc)
add_subdirectory(inc_finder)
add_subdirectory(concordance)
add_subdirectory(cross_ref)
add_subdirectory(sql_analyzer)
