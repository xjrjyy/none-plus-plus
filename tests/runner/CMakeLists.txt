add_executable(runner_test test.cc)

target_link_libraries(runner_test PRIVATE 
    catch
    npp_test
    # runner
)

add_test(runner runner_test)
