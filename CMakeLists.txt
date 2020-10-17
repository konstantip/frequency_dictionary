cmake_minimum_required(VERSION 3.12)

project(words_counter)

add_compile_options(-W -Wall -pedantic)

find_package(Boost)

if(NOT Boost_FOUND)
  message(FATAL "Boost was not found")
endif()

add_executable(${CMAKE_PROJECT_NAME} main.cpp frequency_dictionary/frequency_dictionary.cpp)

set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
