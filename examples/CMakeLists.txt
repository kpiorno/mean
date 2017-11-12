add_compile_options(-std=c++11)
add_executable (simple simple.cpp)
target_include_directories (simple PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries (simple LINK_PUBLIC mean)