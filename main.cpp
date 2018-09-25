//
// Created by Kishwar Shafin on 9/24/18.
//

#include <iostream>
#include <string>
#include "modules/headers/fastqreader.h"
#include "modules/headers/read.h"
#include "modules/headers/runlength_encoder.h"
using namespace std;

int main(int argc, char **argv) {
    string file_name = "/Users/kishwar/Kishwar/software/napper/testdata/R1.fq";
    FastqReader fastq_reader(file_name);

    while(1) {
        Read* read = fastq_reader.read();
        if(read == NULL){
            break;
        }else{
            Compressed_Read* compressed_read = RunLength_Encoder(read);
            cout<<compressed_read->read_name<<endl;
        }
    }
    return 0;
}

