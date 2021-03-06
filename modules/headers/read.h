// REUSE NOTICE: https://github.com/OpenGene/fastp
#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "sequence.h"
#include <vector>

using namespace std;

typedef struct Compressed_Read{
	string read_name;
	string compressed_seq;
	string compressed_quality;
	string strand_direction;
	vector<int> run_lengths;
	Compressed_Read(string _name, string _seq, string _quality, string _direction, vector<int> _run_lengths){
		read_name = _name;
		compressed_seq = _seq;
		compressed_quality = _quality;
		strand_direction = _direction;
		run_lengths = _run_lengths;
	}
} Compressed_Read;


typedef struct Compressed_Fasta_Read{
	string compressed_seq;
	vector<int> run_lengths;
	Compressed_Fasta_Read(string _seq, vector<int> _run_lengths){
		compressed_seq = _seq;
		run_lengths = _run_lengths;
	}
} Compressed_Fasta_Read;

class Read{
public:
	Read(string name, string seq, string strand, string quality, bool phred64=false);
    Read(string name, Sequence seq, string strand, string quality, bool phred64=false);
	Read(string name, string seq, string strand);
    Read(string name, Sequence seq, string strand);
    Read(Read &r);
	void print();
    void printFile(ofstream& file);
    Read* reverseComplement();
    string firstIndex();
    string lastIndex();
    // default is Q20
    int lowQualCount(int qual=20);
    int length();
    string toString();
    void resize(int len);
    void convertPhred64To33();
    void trimFront(int len);

public:
    static bool test();

private:


public:
	string mName;
	Sequence mSeq;
	string mStrand;
	string mQuality;
	bool mHasQuality;
};

class ReadPair{
public:
    ReadPair(Read* left, Read* right);
    ~ReadPair();

    // merge a pair, without consideration of seq error caused false INDEL
    Read* fastMerge();
public:
    Read* mLeft;
    Read* mRight;

public:
    static bool test();
};

#endif