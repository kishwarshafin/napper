//
// Created by Kishwar Shafin on 9/24/18.
//
#include "gmock/gmock.h"
#include "gtest/gtest.h"


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return ret;
}