The purpose of the program is to find a word in a huge text file (was intended for logs). It can find the exact word or variatians using regular expressions, as follows:
'*' character - a sequence of any characters of unlimited length;
'?' character - any single character;

1. Mask *abc* selects all strings that contain 'abc', beginning and ending with any sequence of characters.
Same for *abc***, **abc**, **abc* - any number of *

2. Mask abc* selects all strings beginning with 'abc' and ending with any sequence of characters.
Same for abc**, abc***

3. Mask abc? selects all strings beginning with 'abc' and ending with any additional character. '?' Refers to strictly
one character. Same for ?abc, ??abc, abc?, abc??, ??abc??? any number of ? implemented.

4. Mask abc selects all strings that are equal to this mask. Normal find function.

5. Mixing '*' and '?' : 
?abc* with any number of ? and *
?abc*, ???abc*** , *abc?, *abc??? similar

6. If you are unsure of how a word may have been spelled by the author, I have implemented
ab?c, ab???c with any number of '?' . Thought of Anatolia /Anatolya/Anatholia or Ciprus/Cyprus when writing this feature.

The program creates С++ class, which is able to read large text log files (dozens and hundreds of megabytes) and output strings that satisfy the conditions of the simplest regular expression.


The program is designed to be run in CLI using the format: regular_expression logfilename mask
MAke sure you have the log file in the same folder as the .exe (under the current form above, without warrying about path)

'space' as any white space delimiter is used for separating words when reading from the file.

For testing I have used a log file combined of hijackthis and some other security program, can't provide it because it contains
all kinds of infos about my system.

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

I think I should have countet the lines where and print the line from the log next to the found word.
