#include "CLogReader.h"
#include <iostream>


CLogReader::CLogReader(const char* logfile, const char *filter)
{
	//std::cout << "intra in constructor" << std::endl;
	filename = (char*)calloc(250, sizeof(char)); //filename should not exceed 250 characters
	strcpy(filename, logfile);

	//std::cout << filename << std::endl;
	Open(filename);
	SetFilter(filter);
}

CLogReader::~CLogReader()
{
	free(filename);
	free(mask);
}

bool CLogReader::Open(const char * logfile)
{
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		std::cout << "Invalid filename";
		exit(1);
		return false;
	}
	//std::cout << "file " << filename << " openend for reading" << std::endl;
	return true;
}

void CLogReader::Close()
{
	fclose(fp);
}

bool CLogReader::SetFilter(const char * filter)
{

	if (filter == NULL || strlen(filter) == 0)
	{
		std::cout << "invalid mask";
		return false;
	}
	
	mask = (char*)calloc(1000, sizeof(char)); //mask should not exceed 1000 characters
											  //chosen if you intend to search for a path for example
	strcpy(mask, filter);
	//std::cout << mask << std::endl;
	return true;
}

bool CLogReader::GetNextLine(char * buf, const int bufsize)
{
	
	/* assumes no word exceeds length of 1023 */
	if (fscanf(fp, " %s", buf) == 1)
		return true;
	
	return false;
}

void CLogReader::run()
{

	char word[MAXCHARS_WORD];
	int start_star = 0;
	int end_star = 0;
	int cursor = 0;
	int lastFS = 0; //position of the last star in front
	int firstLS = 0; //position of the first trailing star

	//int question_b = 0; //check whether we have ? in the back, no longer necessary
	int nr_question_b = 0; //number of question marks on the back part of the mask
	int nr_question_f = 0; //number of question marks at the front of the mask
	int nr_question_m = 0; //number of question marks in the middle of the mask 
	int series_q = 0; //mark if we are currently counting a series of '?'
	int lastFQ = 0; //position of the last ? in front
	int firstMQ = 0; //position of the ? in the middle
	int firstLQ = 0; //position of the first trailing star

	//check for * at the beginning
	while (mask[cursor] == '*')
	{
		 start_star= 1;
		 lastFS = cursor;
		 cursor++; //doesn't matter how many * are in front

	}
	//check for ? at the beginning
	while (mask[cursor] == '?')
	{
		//start_star = 1;
		nr_question_f++;
		lastFQ = cursor;
		cursor++; 

	}

	//skip non-asterisk characters
	while (cursor < strlen(mask)  && mask[cursor] != '*' )
	{
		if (mask[cursor] == '?')
		{
			if (series_q == 0)
				series_q = 1;

			nr_question_b++;
			if (firstLQ == 0)
				firstLQ = cursor;
		}
		else //normal character
		{
			if (series_q == 1) //we had a series of ?, means we have ab??c
			{
				nr_question_m = nr_question_b;
				firstMQ = firstLQ;
				//reset the values that mark '?' at the end, for a future ab??c? implementation
				nr_question_b = 0;
				firstLQ = 0;
				series_q = 0;
			}
		}
		
		

		cursor++;//continue analysing the mask
		
	}
	//check for * at the end
 	while (cursor < strlen(mask) && mask[cursor] == '*')
	{
		end_star = 1;
		if (firstLS == 0)
			firstLS = cursor;
		cursor++;
	}
	//std::cout << "start_star = " << start_star << ", end_star = " << end_star << std::endl;
	
	//cleaning the mask for comparisson
	//remove the * from the mask

	// stars in front
	if (start_star == 1)
	{
		for (int i = lastFS; i < strlen(mask); i++)
			mask[i - lastFS] = mask[i + 1];
	}
	// stars behind characters
	if (end_star == 1)
	{
		if(start_star == 1 )
			mask[firstLS - lastFS - 1] = '\0';
		else 
			mask[firstLS] = '\0';
	}

	//remove the ? from the front of the mask
	if(nr_question_f > 0)
	{
		for (int i = lastFQ; i < strlen(mask); i++)
			mask[i - lastFQ] = mask[i + 1];
	}
	//remove question mark from middle of the mask
	if (nr_question_m > 0)
	{
		for(int j = nr_question_m; j > 0; j--)
			for (int i = firstMQ; i < strlen(mask); i++)
				mask[i] = mask[i + 1];

	}
	//remove the ? from the back of the mask
	if (nr_question_b > 0)
	{
		if( nr_question_f == 0 && start_star == 0) //no shifting left of the mask occured
			mask[firstLQ] = '\0';
		else
			mask[firstLQ - lastFQ - 1] = '\0';
	}
	
	//std::cout << "semnul ? apare de " << nr_queston_b << std::endl;

	//std::cout << "mask este " << mask << std::endl;
	//starting matching and printing
	while (GetNextLine(word, MAXCHARS_WORD) != false) //while there are strings in the log file
	{
		//simple cases *abc*, abc, *abc, abc* , no ?
		if (nr_question_b == 0 && nr_question_f == 0)
		{
			//case *abc*
			if (start_star == 1 && end_star == 1)
				if (strstr(word, mask) != NULL)
					std::cout << word << std::endl;

			//case abc
			if (start_star == 0 && end_star == 0)
				if (strcmp(word, mask) == 0)
					std::cout << word << std::endl;

			//case abc*
			if (start_star == 0 && end_star == 1)
				if (strncmp(word, mask, strlen(mask)) == 0)
					std::cout << word << std::endl;

			//case *abc
			if (start_star == 1 && end_star == 0)
				if (strlen(word) >= strlen(mask) &&
					0 == strcmp(word + (strlen(word) - strlen(mask)), mask))
					std::cout << word << std::endl;
		}

		//simple cases with ? only
		if (start_star == 0 && end_star == 0) //no *
		{
			//case abc???
			if (nr_question_b > 0 && nr_question_f == 0)
				if (start_star == 0)
					if (strncmp(word, mask, strlen(mask)) == 0 &&
						strlen(word) == strlen(mask) + nr_question_b)
						std::cout << word << std::endl;

			//case ???abc
			if (nr_question_f > 0 && nr_question_b == 0)
				if (strlen(word) == strlen(mask) + nr_question_f &&
					0 == strcmp(word + (strlen(word) - strlen(mask)), mask))
					std::cout << word << std::endl;

			//case ??abc??
			if (nr_question_f > 0 && nr_question_b > 0)
			{
				//std::cout << "? fata " << nr_question_f << " ? spate " << nr_question_b << std::endl;
				//std::cout << mask << std::endl;

				if (strlen(word) == strlen(mask) + nr_question_f + nr_question_b &&
					0 == strncmp(word + (strlen(word) - strlen(mask) - nr_question_b), mask, strlen(mask)))
					std::cout << word << std::endl;
			}
			//case ab?c
			if (nr_question_m > 0)
			{
				
				//std::cout << "intra aici" << std::endl;
				int FPlength = firstMQ + nr_question_m; //length of the first part 
				
				if (strncmp(word, mask, firstMQ) == 0 &&
					strlen(word) == strlen(mask) + nr_question_m &&
					strncmp(word + FPlength, mask + firstMQ, strlen(word) - FPlength) == 0)
				{
					//std::cout << "word for final cmp is " << word + FPlength << std::endl;
					//std::cout << "mask for final cmp is " << mask + firstMQ << std::endl;
					std::cout << word << std::endl;
				}
			}
		}
		//case *abc?, *abc???, **abc???
		if (start_star != 0 && end_star == 0 && nr_question_b > 0)
		{
			//std::cout << "intra aici" << std::endl;
			//std::cout << mask << std::endl;
			if (strlen(word) >= strlen(mask) + nr_question_b &&
				0 == strncmp(word + (strlen(word) - strlen(mask) - nr_question_b), mask, strlen(mask)))
				std::cout << word << std::endl;
		}
		//case ?abc*, ??abc***
		if (start_star == 0 && end_star != 0 && nr_question_f > 0)
		{
			//static int contor = 0;
			
			if (strncmp(word + nr_question_f, mask, strlen(mask)) == 0 && 
				strlen(word) >= strlen(mask) + nr_question_f )
			{
				//contor++;
				std::cout << word <<  std::endl;
			}
		}

	}
}
