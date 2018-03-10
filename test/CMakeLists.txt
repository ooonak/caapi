include_directories(${CMAKE_SOURCE_DIR}/src)
add_executable(${PROJECT_NAME}_test unittest_cautils.cpp)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_lib ${OPENSSL_LIBRARIES} gtest_main)
