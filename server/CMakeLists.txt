set(HEADERS dbg.hpp certutils.hpp cautils.hpp crow_all.h server.hpp)
set(SOURCES certutils.cpp cautils.cpp server.cpp)

add_library(${PROJECT_NAME}_server_lib ${HEADERS} ${SOURCES})
add_executable(${PROJECT_NAME}_server main.cpp)

target_link_libraries(${PROJECT_NAME}_server ${PROJECT_NAME}_server_lib)
target_link_libraries(${PROJECT_NAME}_server ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_server ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES})

if (Tcmalloc_FOUND)
        target_link_libraries(${PROJECT_NAME}_server ${Tcmalloc_LIBRARIES})
endif(Tcmalloc_FOUND)
