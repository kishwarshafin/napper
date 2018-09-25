// REUSE NOTICE: https://github.com/OpenGene/fastp
#ifndef FASTA_READER_H
#define FASTA_READER_H

#include <stdio.h>
#include <stdlib.h>
#include "read.h"
#include "zlib/zlib.h"
#include "common.h"
#include <iostream>
#include <fstream>

class FastaReader{
public:
	FastaReader(string filename);
	~FastaReader();
	bool isZipped();

	void getBytes(size_t& bytesRead, size_t& bytesTotal);

	//this function is not thread-safe
	//do not call read() of a same FastqReader object from different threads concurrently
	string read();
	bool eof();
	bool hasNoLineBreakAtEnd();

public:
	static bool isZipFasta(string filename);
	static bool isFasta(string filename);
	static bool test();

private:
	void init();
	void close();
	string getLine();
	void clearLineBreaks(char* line);
	void readToBuf();

private:
	string mFilename;
	gzFile mZipFile;
	FILE* mFile;
	bool mZipped;
	char* mBuf;
	int mBufDataLen;
	int mBufUsedLen;
	bool mStdinMode;
	bool mHasNoLineBreakAtEnd;

};

#endif