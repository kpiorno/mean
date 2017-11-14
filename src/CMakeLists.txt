add_compile_options(-std=c++11)
add_library (mean mn_parser.cpp mn_treegen.cpp mn_lexer.cpp 
             mn_error_msg.cpp mn_mn_lexer.cpp mn_data_struct.cpp 
             mn_root.cpp mn_token.cpp mn_traveler.cpp
             core/mn_ustream.cpp lexers/mn_simple.cpp
             lexers/mn_python.cpp)
target_include_directories (mean PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../include)