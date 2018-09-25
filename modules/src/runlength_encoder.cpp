//
// Created by Kishwar Shafin on 9/24/18.
//

// This script takes a read and performs run-length encoding on the read.

#include "../headers/runlength_encoder.h"

Compressed_Read* RunLength_Encoder(Read* uncompressed_read) {
    string read_name = uncompressed_read->mName;
    string direction = uncompressed_read->mStrand;
    string read_seq = uncompressed_read->mSeq.mStr;
    string read_quality = uncompressed_read->mQuality;

    string compressed_seq;
    string compressed_quality;
    vector<int> run_length;

    // if the read has quality then compress both
    if(uncompressed_read->mHasQuality) {
        long long uncompressed_read_iterator = 1;
        long long current_run_length_count = 1;
        long long current_quality = int(read_quality[0]);
        char previous_base = read_seq[0];

        // add the first base
        compressed_seq += previous_base;

        while(uncompressed_read_iterator < read_seq.length()) {

            char current_base = read_seq[uncompressed_read_iterator];

            if(current_base == previous_base) {
                current_run_length_count += 1;
                current_quality += int(read_quality[uncompressed_read_iterator]);
            } else {
                // add the next base and the run length of current base
                compressed_seq += current_base;
                // add run length and quality of previous base
                run_length.push_back(current_run_length_count);
                compressed_quality += char(current_quality / current_run_length_count);

                // reset the values
                previous_base = current_base;
                current_run_length_count = 1;
                current_quality = int(read_quality[uncompressed_read_iterator]);
            }
            uncompressed_read_iterator += 1;
        }
        // add the run length of the last observed base
        run_length.push_back(current_run_length_count);
        compressed_quality += char(current_quality / current_run_length_count);
    } else{
        long long uncompressed_read_iterator = 1;
        long long current_run_length_count = 1;
        char previous_base = read_seq[0];

        // add the first base
        compressed_seq += previous_base;

        while(uncompressed_read_iterator < read_seq.length()) {

            char current_base = read_seq[uncompressed_read_iterator];

            if(current_base == previous_base) {
                current_run_length_count += 1;
            } else {
                // add the next base and the run length of current base
                compressed_seq += current_base;
                run_length.push_back(current_run_length_count);

                // reset the values
                previous_base = current_base;
                current_run_length_count = 1;
            }
            uncompressed_read_iterator += 1;
        }
        // add the run length of the last observed base
        run_length.push_back(current_run_length_count);
    }

    return new Compressed_Read(read_name, compressed_seq, compressed_quality, direction, run_length);
}


Compressed_Fasta_Read* RunLength_Encoder_Fasta(string sequence) {
    string compressed_seq;
    vector<int> run_length;

    int uncompressed_read_iterator = 1;
    int current_run_length_count = 1;
    char previous_base = sequence[0];

    // add the first base
    compressed_seq += previous_base;

    while(uncompressed_read_iterator < sequence.length()) {

        char current_base = sequence[uncompressed_read_iterator];

        if(current_base == previous_base) {
            current_run_length_count += 1;
        } else {
            // add the next base and the run length of current base
            compressed_seq += current_base;
            run_length.push_back(current_run_length_count);

            // reset the values
            previous_base = current_base;
            current_run_length_count = 1;
        }
        uncompressed_read_iterator += 1;
    }
    // add the run length of the last observed base
    run_length.push_back(current_run_length_count);

    return new Compressed_Fasta_Read(compressed_seq, run_length);
}