# NAPPER (napper)
Nanopore Alignment Pre-processing Pipeline using Encoded Runlength. <br/>
[![Build Status](https://travis-ci.com/kishwarshafin/napper.svg?branch=master)](https://travis-ci.com/kishwarshafin/napper)

### Description
This program uses a run-length encoding (RLE) scheme on genomic sequences for better variant calling in long read sequencing data.

### Installation guide
#### Requirement: <br/>
* cmake >= 3.8
* g++ >= 5

#### Ubuntu:
```bash
git clone https://github.com/kishwarshafin/napper.git
cd napper
mkdir build
cd build
cmake ..
make
make test
```
This should create two executables. Which can be used for run length encoding of fasta and fastq files:
```bash
mkdir outputs
./fasta_rle --input ../testdata/GRSm.fa --output ./outputs/GRSm_compressed.fa
./fastq_rle --input ../testdata/R1.fq --output ./outputs/R1_compressed.fq
```
### Dependencies
The code heavily re-uses these open source projects: <br/>
* [fastp](https://github.com/OpenGene/fastp)
* [googletest](https://github.com/google/googletest)
* [ArgumentParser](https://github.com/hbristow/argparse)
