set(LLVM_LINK_COMPONENTS
    Support
)

add_npp_tool(npp
    token.cpp
    value.cpp
    functions.cpp
    lexer.cpp
    parser.cpp
    calculator.cpp
    main.cpp
)

# add_subdirectory(config)

target_link_libraries(npp PRIVATE
    # config
    fmt::fmt
    cxxopts
)

target_include_directories(npp PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(npp PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(npp PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
