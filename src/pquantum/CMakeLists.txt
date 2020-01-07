cmake_minimum_required(VERSION 3.0)

add_executable(pquantum
        main.cpp program_options_handler.cpp)
target_link_libraries(pquantum logging error Boost::program_options error parsing)