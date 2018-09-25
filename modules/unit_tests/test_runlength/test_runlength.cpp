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

TEST_F(RunLengthEncodingTest, same_base_compression)
{
    Read* read = new Read("ABCD", "AAAA", "-", "BBEE");
    Compressed_Read* compressed_read = RunLength_Encoder(read);
    //read name
    string expected_read_name = read->mName;

    //read name
    string expected_compressed_sequence = "A";

    //expected strand
    string expected_strand = read->mStrand;

    // quality http://www.asciitable.com/ B=66, E=69
    string expected_compressed_quality;
    expected_compressed_quality += char((66 + 66 + 69 + 69) / 4);

    int run_length_values[] = {4};
    vector<int> expected_rle (run_length_values, run_length_values+1);

    EXPECT_EQ(compressed_read->read_name, expected_read_name);
    EXPECT_EQ(compressed_read->compressed_seq, expected_compressed_sequence);
    EXPECT_EQ(compressed_read->strand_direction, expected_strand);
    EXPECT_EQ(compressed_read->compressed_quality, expected_compressed_quality);
    EXPECT_THAT(compressed_read->run_lengths, ::testing::ContainerEq(expected_rle));
}


TEST_F(RunLengthEncodingTest, easy_compression)
{
    Read* read = new Read("ABCD", "ACCGGTTA", "-", "ABBEE]]/");
    Compressed_Read* compressed_read = RunLength_Encoder(read);
    //read name
    string expected_read_name = read->mName;

    //read name
    string expected_compressed_sequence = "ACGTA";

    //expected strand
    string expected_strand = read->mStrand;

    // quality http://www.asciitable.com/ A=65, B=66, E=69, ]=93, /=47
    string expected_compressed_quality;
    expected_compressed_quality += char(65);
    expected_compressed_quality += char((66 + 66) / 2);
    expected_compressed_quality += char((69 + 69) / 2);
    expected_compressed_quality += char((93 + 93) / 2);
    expected_compressed_quality += char(47);

    int run_length_values[] = {1, 2, 2, 2, 1};
    vector<int> expected_rle (run_length_values, run_length_values+5);

    EXPECT_EQ(compressed_read->read_name, expected_read_name);
    EXPECT_EQ(compressed_read->compressed_seq, expected_compressed_sequence);
    EXPECT_EQ(compressed_read->strand_direction, expected_strand);
    EXPECT_EQ(compressed_read->compressed_quality, expected_compressed_quality);
    EXPECT_THAT(compressed_read->run_lengths, ::testing::ContainerEq(expected_rle));
}