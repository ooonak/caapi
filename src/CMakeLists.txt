set(HEADERS dbg.hpp certutils.hpp cautils.hpp clientutils.hpp)
set(SOURCES certutils.cpp cautils.cpp clientutils.cpp)

add_library(${PROJECT_NAME}_lib ${HEADERS} ${SOURCES})
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib ${OPENSSL_LIBRARIES})

