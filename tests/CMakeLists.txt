add_library(npp_test STATIC EXCLUDE_FROM_ALL main.cc)

target_link_libraries(npp_test PRIVATE catch)

# add_subdirectory(dataconfigparser)

