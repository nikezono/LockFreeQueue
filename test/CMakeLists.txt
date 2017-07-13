add_executable(queue_test queue_test.cpp)
target_link_libraries(queue_test gtest_main)
add_test(queue_test queue_test)
