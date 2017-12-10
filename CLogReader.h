#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

#define MAXCHARS_LINE 1000 //estimated number of characters on a line, for a log file
#define MAXCHARS_WORD 4000 //an estimated number of characters for a word (string not separated by white spaces)
							// for a log file should 

class CLogReader
{
public:
	CLogReader(const char* logfile, const char *filter);
	~CLogReader();

	bool    Open(const char * filename);                       // open a file, false - error
	void    Close();                         // close the file

	bool    SetFilter(const char *filter);   // set the string filters, false - error
	bool    GetNextLine(char *buf,           // request the next found string,
		const int bufsize);  // buf - buffer, bufsize - the maximum length
							 // false - end of file or error
	void	run(); //algorithm to print the words containing the mask

private:
	FILE *fp;
	char *filename;
	char *mask; //the mask used in filtering

};
