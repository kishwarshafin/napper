CMAKE_MINIMUM_REQUIRED(VERSION 3.12.2)

# name of the project is N.A.P.P.E.R (Nanopore Assembly Pre-processing Pipeline using Encoded Runlength)
PROJECT(napper)


set(CXX_STANDARD 11)


# FIND AND SET THE LIBRARY FILES
SET(LIBRARY_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/fastqreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/read.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/sequence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/runlength_encoder.cpp
)

# ADD THE MODULE DIRECTORY
INCLUDE_DIRECTORIES(modules)

# SET UP THE LIBRARY
MESSAGE("LIBRARY_FILES FILES: ${LIBRARY_FILES}")
ADD_LIBRARY(napper_lib ${LIBRARY_FILES})

# COMPILE COMMAND
ADD_EXECUTABLE(napper main.cpp)
# ADD THE LIBRARY TO THE TARGET
TARGET_LINK_LIBRARIES(napper napper_lib)

# LINK TO ZLIB LIBRARY
TARGET_LINK_LIBRARIES(napper z)

#-------------------
# Test
#-------------------
ENABLE_TESTING()
ADD_SUBDIRECTORY(modules/unit_tests)

