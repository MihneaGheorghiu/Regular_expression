#include "CLogReader.h"
#include <iostream>


// argv[0] will be the executable, argv[1] the log file and argv[2] the string with mask
int main(int argc, char* argv[])
{
	
	if (argc < 3)
	{ 
		std::cout << "Usage: program.exe logfile regular_expression";
		exit(1);
	}

	
	//CLogReader Reader("test.log", "Secu??");
	CLogReader Reader(argv[1], argv[2]);
	Reader.run();
	
	Reader.Close();

	//system("PAUSE");//used during testing face, should be removed in final version
	return 0;
}