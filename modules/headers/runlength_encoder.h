//
// Created by Kishwar Shafin on 9/24/18.
//

#ifndef NAPPER_RUNLENGTH_ENCODER_H
#define NAPPER_RUNLENGTH_ENCODER_H

#include "read.h"

Compressed_Read* RunLength_Encoder(Read* uncompressed_read);
Compressed_Fasta_Read* RunLength_Encoder_Fasta(string sequence);

#endif //NAPPER_RUNLENGTH_ENCODER_H
