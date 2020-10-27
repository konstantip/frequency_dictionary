cmake_minimum_required(VERSION 3.12)

project(words_counter)

add_compile_options(-O3 -W -Wall -Werror -pedantic)

set(WORDS_COUNTER_EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})

add_executable(${WORDS_COUNTER_EXECUTABLE_NAME} main.cpp)

set_property(TARGET ${WORDS_COUNTER_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 17)

add_subdirectory(frequency_dictionary)
target_link_libraries(${WORDS_COUNTER_EXECUTABLE_NAME} PRIVATE frequency_dictionary)

target_include_directories(${WORDS_COUNTER_EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

