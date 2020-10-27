cmake_minimum_required(VERSION 3.12)

add_compile_options(-O3 -W -Wall -Werror -pedantic)

set(FREQUENCY_DICTIONARY_LIBRARY_NAME frequency_dictionary)

find_package(Boost)

if(NOT Boost_FOUND)
    message(FATAL "Boost was not found")
endif()

add_library(${FREQUENCY_DICTIONARY_LIBRARY_NAME} STATIC frequency_dictionary.cpp)

set_property(TARGET ${FREQUENCY_DICTIONARY_LIBRARY_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${FREQUENCY_DICTIONARY_LIBRARY_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(${FREQUENCY_DICTIONARY_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
