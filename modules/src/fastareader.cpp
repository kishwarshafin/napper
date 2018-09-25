// REUSE NOTICE: https://github.com/OpenGene/fastp
#include "../headers/fastareader.h"
#include "../headers/util.h"
#include <string.h>

#define FQ_BUF_SIZE (1<<20)

FastaReader::FastaReader(string filename){
	mFilename = filename;
	mZipFile = NULL;
	mZipped = false;
	mFile = NULL;
	mBuf = new char[FQ_BUF_SIZE];
	mBufDataLen = 0;
	mBufUsedLen = 0;
	mHasNoLineBreakAtEnd = false;
	init();
}

FastaReader::~FastaReader(){
	close();
	delete mBuf;
}

bool FastaReader::hasNoLineBreakAtEnd() {
	return mHasNoLineBreakAtEnd;
}

void FastaReader::readToBuf() {
	if(mZipped) {
		mBufDataLen = gzread(mZipFile, mBuf, FQ_BUF_SIZE);
		if(mBufDataLen == -1) {
			cerr << "Error to read gzip file" << endl;
		}
	} else {
		mBufDataLen = fread(mBuf, 1, FQ_BUF_SIZE, mFile);
	}
	mBufUsedLen = 0;

	if(mBufDataLen < FQ_BUF_SIZE) {
		if(mBuf[mBufDataLen-1] != '\n')
			mHasNoLineBreakAtEnd = true;
	}
}

void FastaReader::init(){
	if (ends_with(mFilename, ".gz")){
		mZipFile = gzopen(mFilename.c_str(), "r");
		mZipped = true;
		gzrewind(mZipFile);
	}
	else {
		if(mFilename == "/dev/stdin") {
			mFile = stdin;
		}
		else
			mFile = fopen(mFilename.c_str(), "rb");
		if(mFile == NULL) {
			error_exit("Failed to open file: " + mFilename);
		}
		mZipped = false;
	}
	readToBuf();
}

void FastaReader::getBytes(size_t& bytesRead, size_t& bytesTotal) {
	if(mZipped) {
		bytesRead = gzoffset(mZipFile);
	} else {
		bytesRead = ftell(mFile);//mFile.tellg();
	}

	// use another ifstream to not affect current reader
	ifstream is(mFilename);
	is.seekg (0, is.end);
	bytesTotal = is.tellg();
}

void FastaReader::clearLineBreaks(char* line) {

	// trim \n, \r or \r\n in the tail
	int readed = strlen(line);
	if(readed >=2 ){
		if(line[readed-1] == '\n' || line[readed-1] == '\r'){
			line[readed-1] = '\0';
			if(line[readed-2] == '\r')
				line[readed-2] = '\0';
		}
	}
}

string FastaReader::getLine(){
	int copied = 0;

	int start = mBufUsedLen;
	int end = start;

	while(end < mBufDataLen) {
		if(mBuf[end] != '\r' && mBuf[end] != '\n')
			end++;
		else
			break;
	}

	// this line well contained in this buf, or this is the last buf
	if(end < mBufDataLen || mBufDataLen < FQ_BUF_SIZE) {
		int len = end - start;
		string line(mBuf+start, len);

		// skip \n or \r
		end++;
		// handle \r\n
		if(end < mBufDataLen-1 && mBuf[end] == '\n')
			end++;

		mBufUsedLen = end;

		return line;
	}

	// this line is not contained in this buf, we need to read new buf
	string str(mBuf+start, mBufDataLen - start);

	while(true) {
		readToBuf();
		start = 0;
		end = 0;
		while(end < mBufDataLen) {
			if(mBuf[end] != '\r' && mBuf[end] != '\n')
				end++;
			else
				break;
		}
		// this line well contained in this buf, we need to read new buf
		if(end < mBufDataLen || mBufDataLen < FQ_BUF_SIZE) {
			int len = end - start;
			str.append(mBuf+start, len);

			// skip \n or \r
			end++;
			// handle \r\n
			if(end < mBufDataLen-1 && mBuf[end] == '\n')
				end++;

			mBufUsedLen = end;

			return str;
		}
		// even this new buf is not enough, although impossible
		str.append(mBuf+start, mBufDataLen);
	}

	return string();
}

bool FastaReader::eof() {
	if (mZipped) {
		return gzeof(mZipFile);
	} else {
		return feof(mFile);//mFile.eof();
	}
}

string FastaReader::read(){
    string line;
	if (mZipped){
		if (mZipFile == NULL)
			return line;
	}

	if(mBufUsedLen >= mBufDataLen && eof()) {
		return line;
	}

	line = getLine();

	return line;
}

void FastaReader::close(){
	if (mZipped){
		if (mZipFile){
			gzclose(mZipFile);
			mZipFile = NULL;
		}
	}
	else {
		if (mFile){
			fclose(mFile);//mFile.close();
			mFile = NULL;
		}
	}
}

bool FastaReader::isZipFasta(string filename) {
	if (ends_with(filename, ".fastq.gz"))
		return false;
	else if (ends_with(filename, ".fq.gz"))
		return false;
	else if (ends_with(filename, ".fasta.gz"))
		return true;
	else if (ends_with(filename, ".fa.gz"))
		return true;
	else
		return false;
}

bool FastaReader::isFasta(string filename) {
	if (ends_with(filename, ".fastq"))
		return false;
	else if (ends_with(filename, ".fq"))
		return false;
	else if (ends_with(filename, ".fasta"))
		return true;
	else if (ends_with(filename, ".fa"))
		return true;
	else
		return false;
}

bool FastaReader::isZipped(){
	return mZipped;
}
