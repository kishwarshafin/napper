//
// Created by Kishwar Shafin on 9/24/18.
//

#include <iostream>
#include <vector>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "../../headers/read.h"
#include "../../headers/runlength_encoder.h"


class RunLengthEncodingTest : public ::testing::Test {
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    RunLengthEncodingTest() {
        // You can do set-up work for each test here.
    }

    virtual ~RunLengthEncodingTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor (right
        // before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test (right
        // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for Project1.
};

TEST_F(RunLengthEncodingTest, simplest_test)
{
    Read* read = new Read("AA:BB:CC", "ACTG", "+", "AAEE");
    Compressed_Read* compressed_read = RunLength_Encoder(read);

    int run_length_values[] = {1, 1, 1, 1};
    vector<int> expected_rle ( run_length_values, run_length_values+4 );

    EXPECT_EQ(compressed_read->read_name, "AA:BB:CC");
    EXPECT_EQ(compressed_read->compressed_seq, "ACTG");
    EXPECT_EQ(compressed_read->strand_direction, "+");
    EXPECT_EQ(compressed_read->compressed_quality, "AAEE");
    EXPECT_THAT(compressed_read->run_lengths, ::testing::ContainerEq(expected_rle));
}

TEST_F(RunLengthEncodingTest, no_read_name_test)
{
    Read* read = new Read("", "ACTG", "+", "AAEE");
    Compressed_Read* compressed_read = RunLength_Encoder(read);

    int run_length_values[] = {1, 1, 1, 1};
    vector<int> expected_rle ( run_length_values, run_length_values+4 );

    EXPECT_EQ(compressed_read->read_name, "");
    EXPECT_EQ(compressed_read->compressed_seq, "ACTG");
    EXPECT_EQ(compressed_read->strand_direction, "+");
    EXPECT_EQ(compressed_read->compressed_quality, "AAEE");
    EXPECT_THAT(compressed_read->run_lengths, ::testing::ContainerEq(expected_rle));
}