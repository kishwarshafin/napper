file(GLOB SRCS *.cpp)

message(${SRCS})
add_executable(test_runlength ${SRCS})

target_link_libraries(test_runlength napper_lib libgtest libgmock)

add_test(NAME RUNLENGTH_ENCODING_TEST COMMAND test_runlength)