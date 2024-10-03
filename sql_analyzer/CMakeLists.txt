set(target_name sql_analyzer)

FLEX_TARGET(scanner
    scanner.lpp
    ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp
    COMPILE_FLAGS $<$<STREQUAL:${CMAKE_CXX_COMPILER_ID},MSVC>:--wincompat>
)

BISON_TARGET(parser
    parser.ypp
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
    COMPILE_FLAGS -Wcounterexamples
)

ADD_FLEX_BISON_DEPENDENCY(scanner parser)

add_executable(${target_name}
    main.cpp
    ${FLEX_scanner_OUTPUTS}
    ${BISON_parser_OUTPUTS}
)

target_include_directories(${target_name}
  PRIVATE
    ${FLEX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_options(${target_name}
    PRIVATE
        $<$<STREQUAL:${CMAKE_CXX_COMPILER_ID},MSVC>:/Zc:__cplusplus>
)
