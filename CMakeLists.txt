CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(phspider)

IF(DEBUG)
    MESSAGE(STATUS "Setting build type to 'Debug'.")
    SET(CMAKE_BUILD_TYPE Debug)
ELSE()
    MESSAGE(STATUS "Setting build type to 'Release'.")
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../)
SET(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/../)

ADD_DEFINITIONS("-std=c++0x -fpermissive")

SET(INCLUDE_PATH /usr/local/include)
INCLUDE_DIRECTORIES(/usr/local/include)

SET(LINK_PATH /usr/local/lib)
LINK_DIRECTORIES(/usr/local/lib)

ADD_SUBDIRECTORY(utils)
ADD_SUBDIRECTORY(unitest)

