cmake_minimum_required(VERSION 3.12)

project(words_counter)

add_compile_options(-O3 -Wall -Werror -pedantic)

set(WORDS_COUNTER_EXECUTABLE_NAME ${CMAKE_PROJECT_NAME})

add_executable(${WORDS_COUNTER_EXECUTABLE_NAME} main.cpp)

set_property(TARGET ${WORDS_COUNTER_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 17)

set(VERSION 0)

if (VERSION EQUAL 0)
    add_subdirectory(v0/frequency_dictionary)
else()
    add_subdirectory(v1/frequency_dictionary)
endif()

target_link_libraries(${WORDS_COUNTER_EXECUTABLE_NAME} PRIVATE frequency_dictionary)


