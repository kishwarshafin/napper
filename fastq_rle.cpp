//
// Created by Kishwar Shafin on 9/24/18.
//

#include <iostream>
#include <string>
#include "modules/headers/fastqreader.h"
#include "modules/headers/read.h"
#include "modules/headers/runlength_encoder.h"
#include "modules/headers/writer.h"
#include "modules/headers/argparse.h"
using namespace std;

void run_length_fastq(string input_file_name, string output_file_name) {
    FastqReader fastq_reader(input_file_name);
    Writer output_writer(output_file_name);
    Writer output_writer_rle(output_file_name+".rl");
    long long read_count = 0;
    while(true) {
        Read* read = fastq_reader.read();
        if(read_count % 10000 == 0 and read_count > 0){
            cerr<<read_count <<" READS PROCESSED." << endl;
        }
        if(read == nullptr) {
            break;
        } else {
            Compressed_Read* compressed_read = RunLength_Encoder(read);
            output_writer.writeLine(compressed_read->read_name);
            output_writer.writeLine(compressed_read->compressed_seq);
            output_writer.writeLine(compressed_read->strand_direction);
            output_writer.writeLine(compressed_read->compressed_quality);

            output_writer_rle.writeLine(compressed_read->read_name);
            string run_length_line;
            for(int i=0; i<compressed_read->run_lengths.size(); i++){
                if(i==0){
                    run_length_line += to_string(compressed_read->run_lengths[i]);
                }else{
                    run_length_line += ",";
                    run_length_line += to_string(compressed_read->run_lengths[i]);
                }
            }
            output_writer_rle.writeLine(run_length_line);
            read_count += 1;
        }
    }
}


int main(int argc, const char **argv) {
    // make a new ArgumentParser
    ArgumentParser parser;

    // add some arguments to search for
    parser.addArgument("-i", "--input", 1, true);
    parser.addArgument("-o", "--output", 1, true);

    // parse the command-line arguments - throws if invalid format
    parser.parse(argc, argv);

    // if we get here, the configuration is valid
    string input_file = parser.retrieve<string>("input");
    string output_file = parser.retrieve<string>("output");

    run_length_fastq(input_file, output_file);
    return 0;
}

