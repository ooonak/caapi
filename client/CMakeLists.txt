set(HEADERS dbg.hpp certutils.hpp clientutils.hpp client.hpp)
set(SOURCES certutils.cpp clientutils.cpp client.cpp)

add_library(${PROJECT_NAME}_client_lib ${HEADERS} ${SOURCES})
add_executable(${PROJECT_NAME}_client main.cpp)
target_link_libraries(${PROJECT_NAME}_client ${PROJECT_NAME}_client_lib ${OPENSSL_LIBRARIES})

