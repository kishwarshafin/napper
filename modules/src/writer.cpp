#include "../headers/writer.h"
#include "../headers/util.h"
#include "../headers/fastqreader.h"
#include <string.h>

Writer::Writer(string filename, int compression){
	mCompression = compression;
	mFilename = filename;
	mZipFile = NULL;
	mZipped = false;
	haveToClose = true;
	init();
}

Writer::Writer(ofstream* stream) {
	mZipFile = NULL;
	mZipped = false;
	mOutStream = stream;
	haveToClose = false;
}

Writer::Writer(gzFile gzfile) {
	mOutStream = NULL;
	mZipFile = gzfile;
	mZipped = true;
	haveToClose = false;
}

Writer::~Writer(){
	if(haveToClose) {
		close();
	}
}

string Writer::filename(){
	return mFilename;
}

void Writer::init(){
	if (ends_with(mFilename, ".gz")){
		mZipFile = gzopen(mFilename.c_str(), "w");
        gzsetparams(mZipFile, mCompression, Z_DEFAULT_STRATEGY);
        gzbuffer(mZipFile, 1024*1024);
		mZipped = true;
	}
	else {
		mOutStream = new ofstream();
		mOutStream->open(mFilename.c_str(), ifstream::out);
		mZipped = false;
	}
}


bool Writer::writeLine_fasta(string& linestr){
	vector<string> lines;
	// split the line into smaller units like fasta standard
	for (int i = 0; i < linestr.length(); i += 70) {
		lines.push_back(linestr.substr(i, 70));
	}

    bool status;
	for(int i=0; i<lines.size(); i++) {
		if(lines[i].empty())
			continue;

		const char *line = lines[i].c_str();
		size_t size = lines[i].length();
		size_t written;

		if (mZipped) {
			written = gzwrite(mZipFile, line, size);
			gzputc(mZipFile, '\n');
			status = size == written;
		} else {
			mOutStream->write(line, size);
			mOutStream->put('\n');
			status = !mOutStream->fail();
		}
	}
	return status;
}


bool Writer::writeLine(string& linestr){
    bool status;
    const char *line = linestr.c_str();
    size_t size = linestr.length();
    size_t written;

    if (mZipped) {
        written = gzwrite(mZipFile, line, size);
        gzputc(mZipFile, '\n');
        status = size == written;
    } else {
        mOutStream->write(line, size);
        mOutStream->put('\n');
        status = !mOutStream->fail();
    }
    return status;
}

bool Writer::writeString(string& str){
	const char* strdata = str.c_str();
	size_t size = str.length();
	size_t written;
	bool status;
	if(mZipped){
		written = gzwrite(mZipFile, strdata, size);
		status = size == written;
	}
	else{
		mOutStream->write(strdata, size);
		status = !mOutStream->fail();
	}

	return status;
}

bool Writer::write(char* strdata, size_t size) {
	size_t written;
	bool status;
	
	if(mZipped){
		written = gzwrite(mZipFile, strdata, size);
		status = size == written;
	}
	else{
		mOutStream->write(strdata, size);
		status = !mOutStream->fail();
	}
	return status;
}

void Writer::close(){
	if (mZipped){
		if (mZipFile){
			gzflush(mZipFile, Z_FINISH);
			gzclose(mZipFile);
			mZipFile = NULL;
		}
	}
	else if(mOutStream) {
		if (mOutStream->is_open()){
			mOutStream->flush();
			//TODO: following two lines will cause crash
			//mOutStream->close();
			//delete mOutStream;
			mOutStream = NULL;
		}
	}
}

bool Writer::isZipped(){
	return mZipped;
}