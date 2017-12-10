I have implemented the class based on what I understood from the description. 

A pure C++ meant a C++ class that only uses it's input arguments, given once (like a pure function) or without using
 normal C functions (calloc, malloc or reading functions)?

 I didn't used std::string since it can be considered part of the STL.

 I implemented * and ? in the way I remembered them from college, * being zero(empty), one or any character string,
  while ? being strictly one

 I respected the class functions as instructed, I would have normally not used those functions.

The program is designed to be run in CLI using the format: regular_expression logfilename mask

'space' as any white space delimiter is used for separating words when reading from the file.

For testing I have used a log file combined of hijackthis and some other security program.

What remains to be done: 
-replace some deprecated functions that give C4996, because of vulnerability to overflow or other issues 
-rearange the code in the part with the many cases when applying the mask in the CLogReader::run() function, too many if
statements, both for looks & an organized way for the program to cover all cases

And of course more tests, and special cases that may cause the program to crash, things I missed while writing the code.

So far I have implemented the following base cases, one by one:
abc, *abc*, **abc***, *abc, ***abc, abc*, abc**
?abc, ??abc, abc?, abc??, ??abc???
*abc?, *abc???
?abc*, ???abc***
ab?c, ab???c

I must also add more cases and generalise the solution.

I will add the results I get from running the program in CLI, in a .txt file, test cases for each such mask, with 
the log file i used for testing

The program only runs with the test cases above, not other combinations of the ? * operators. Need to add other features.
Wasn't tested enough and with larger file. I think I should have countet the lines where and print the line from the log
next to the found word.
