CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

# name of the project is N.A.P.P.E.R (Nanopore Assembly Pre-processing Pipeline using Encoded Runlength)
PROJECT(napper)


SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# FIND AND SET THE LIBRARY FILES
SET(LIBRARY_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/fastqreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/fastareader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/read.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/sequence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/runlength_encoder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/src/writer.cpp
)

# ADD THE MODULE DIRECTORY
INCLUDE_DIRECTORIES(modules)

# SET UP THE LIBRARY
MESSAGE("LIBRARY_FILES FILES: ${LIBRARY_FILES}")
ADD_LIBRARY(napper_lib ${LIBRARY_FILES})

# COMPILE COMMAND FOR FASTQ RUN-LENGTH ENCODER
ADD_EXECUTABLE(fastq_rle fastq_rle.cpp)
# COMPILE COMMAND FOR FASTA RUN-LENGTH ENCODER
ADD_EXECUTABLE(fasta_rle fasta_rle.cpp)

# ADD THE LIBRARY TO THE TARGET
TARGET_LINK_LIBRARIES(fastq_rle napper_lib)
TARGET_LINK_LIBRARIES(fasta_rle napper_lib)

# LINK TO ZLIB LIBRARY
TARGET_LINK_LIBRARIES(fastq_rle z)
TARGET_LINK_LIBRARIES(fasta_rle z)

#-------------------
# Test
#-------------------
# ENABLE_TESTING()
# ADD_SUBDIRECTORY(modules/unit_tests)

