//
// Created by Kishwar Shafin on 9/24/18.
//

#include <iostream>
#include <string>
#include "modules/headers/fastareader.h"
#include "modules/headers/read.h"
#include "modules/headers/runlength_encoder.h"
#include "modules/headers/writer.h"
#include "modules/headers/argparse.h"
using namespace std;


void run_length_fasta(string input_file_name, string output_file_name) {
    FastaReader fasta_reader(input_file_name);
    Writer output_writer(output_file_name);
    Writer output_writer_rle(output_file_name+".rl");

    string sequence_name;
    string entire_sequence;

    while(true) {
        string line = fasta_reader.read();
        if(line.empty() || line[0] == '>') {
            //dump previous sequence
            if(! entire_sequence.empty()) {
                Compressed_Fasta_Read *compressed_read = RunLength_Encoder_Fasta(entire_sequence);
                output_writer.writeLine(sequence_name);
                output_writer.writeLine_fasta(compressed_read->compressed_seq);

                output_writer_rle.writeLine(sequence_name);
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
                cerr<<"FINISHED PROCESSING: "<<sequence_name<<endl;
                delete compressed_read;
            }
            // re initalize the strings
            sequence_name = line;
            entire_sequence = "";
            if(! line.empty())
                cerr<<"STARTING TO PROCESS: "<<sequence_name<<endl;

        }

        if(line.empty()){
            break;
        }
        else if(line[0] != '>') {
            // append sub sequence to create the entire sequence
            entire_sequence += line;
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

    run_length_fasta(input_file, output_file);
    return 0;
}

