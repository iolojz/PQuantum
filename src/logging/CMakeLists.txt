cmake_minimum_required(VERSION 3.0)

add_library(logging SHARED
        logging.cpp)
target_link_libraries(logging Boost::log)
