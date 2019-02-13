/*		Chase Heath	cph794
This is my implementation of the Word Count command that contains the flags -l,
-w, -c, -C, -N. They represent in order number of lines, number of words,
number of characters, text without single-line C comments, and number of
numbers. I split my program into four parts which are main, wc_default, wc_num,
and wc_rmComments. 

The first function that I wrote is the main function. The main purpose of this
function is to set the flags for the Word Count function and then call the 
appropriate function. It starts with every flag being set to zero, then it
loops through the arguments until it hits a '-'. Once it hits that character,
it decided which flag is to be set based on what follows the '-'. Once it has
finished parsing the command line arguments, it checks if there wasn't a flag
set. If there wasn't, then it sets the default flags of charFlag, wordFlag,
and lineFlag. After that, it checks if the numFlag was set, if so it calls
the wc_num function. If not, it checks to see if the commentFlag was set, 
if so it calls the wc_rmComments function. Otherwise it calls the wc_default
function.  

The next function is the wc_rmComments function which takes in three flags, 
the lineFlag, wordFlag, and charFlag. It takes in the next character from STDIN
in a while loop until the feof(stdin) call returns zero, meaning it is the end
of the stream. I had to use this function instead of checking if current == EOF
because binary files can have EOF in their data but not be at the end of the
file. It runs the variable current through a switch statement to determine what
to do. If it is a line feed character, it adds to l and c. It sets a couple
flags to say that it is no longer a comment and line feed is not a character in
a word. It checks to see if c has been added too much by checking lastSlash and
then breaks. In the other whitespace cases of horizontal tab, vertical tab, form
feed, carriage return, and space, it checks to see if it is in a comment. If it
isn't it will say that it isn't a word character and will add to c. If it is not
those characters, it will check if it is a printable non-whitespace character.
It then checks if it is in a comment. If it is it continues to the next
iteration, if not it will check if the current is '/'. If it is then it will
check if the previous current was a slash and if so will set comment = 1. 
Otherwise it will set the flag lastSlash = 1. If the current isn't '/', then
it checks if lastChar == 0. If so, it checks if lastSlash == 1 and comment == 0
and sets maybeWord = 1 if so. Otherwise it will check if maybeWord == 1, if so
add to w and set maybeWord = 0 else just add to w. It will then check if current
!= '/', if it isn't it will set lastSlash = 0 and maybeWord = 0. At the end of
the while loop it checks if comment == 0, and if it is then add to c. Once the
loop has finished it then prints out the number of lines, words, and characters
based on which flags were passed in as 1.

The next function is the wc_num function which also takes in zero flags. 
It takes in the next character from STDIN in awhile loop until the feof(stdin) 
call returns zero, meaning it is at the end of the stream. For each iteration 
of current, it checks of it is a number is within 0 and 9 inclusive. If so it
will set lastNum = 1 which states that a number has begun. Otherwise it will 
check if lastNum == 1, and add to num if so. It will then print out the number
of numbers.

The last function is the wc_default function which is very similar to the
wc_rmComments function. It takes in the same flags and does the same while loop.
In it's switch statement it will react depending on which value current is. If
current is line feed then it will add to l and c, then set lastChar = 0. If it
is any other whitespace character that has been aforementioned it will just add
to c and set lastChar = 0. If current equals anything else if will then check
if it is a printable non-whitespace character, if so it will then check if the
lastChar = 0. If so then it will add to w. Regardless of that it will then set
lastChar = 1. It will always add to c. Once the while loop has finished, it will
print out the number of lines, words, and characters depending on the flags
passed in.

I made the decision that if both -N and -C were set, it would only
take the -N flag because I believe it to be more likely that someone wants
the number of numbers more than the count of characters/words/lines in a file
that doesn't contain comments. I also made the decision to make -N exclusive 
from any other flag so to not create any confusion on the printouts with
ordering. I have ran my program against the actual wc and believe it to be an
accurate recreation of this command.
*/

#include <stdio.h>
#include <stdlib.h>

void wc_rmComments(short int, short int, short int);
void wc_num();
void wc_default(short int, short int, short int);

int main(int argc, char *argv[])
{
	int i = 0;
	short int charFlag = 0, wordFlag = 0, lineFlag = 0;
	short int commentFlag = 0, numFlag = 0;
	while(i < argc)
	{
		if(*argv[i] == '-')
			switch(*(argv[i] + 1))
			{		

				case 'c' :
					charFlag = 1;
					break;
				case 'w' :
					wordFlag = 1;
					break;
				case 'l' :
					lineFlag = 1;
					break;
				case 'N':
					numFlag = 1;
					break;
				case 'C' :
					commentFlag = 1;
					break;
				default:
					lineFlag = 1;
					charFlag = 1;
					wordFlag = 1;
			}
		i++;
	}
	if((!charFlag) && (!wordFlag) && (!lineFlag) && (!numFlag))
		{
			charFlag = 1;
			wordFlag = 1;
			lineFlag = 1;
		}

	if(numFlag)
		wc_num();
	else if(commentFlag)
		wc_rmComments(lineFlag, wordFlag, charFlag);
	else
		wc_default(lineFlag, wordFlag, charFlag);
	return 0;
}

void wc_rmComments(short int lineFlag, short int wordFlag, short int charFlag)
{
int c = 0, w = 0, l = 0;
short int lastChar = 0, lastSlash = 0, comment = 0, maybeWord = 0;
	char current;
	while(current = getchar(), !feof(stdin))
	{

		switch(current)
		{
			case 0xA :		//line feed
				l++;
				c++;
				lastChar = 0;
				comment = 0;
				if(lastSlash == 1)
					c--;
				lastSlash = 0;
				break;
			case 0x9:		//horizontal tab
			case 0xB:		//vertical tab
			case 0xC:		//form feed
			case 0xD:		//carriage return
			case 0x20:		//space
				if(!comment)
				{	
					lastChar = 0;
					c++;
					break;
				}
			default :
				if(current > 0x20 && current != 0x7F)
				{
					if(!comment)
					{
						if(current == 0x2F)
						{
							if(lastSlash)
								comment = 1;
							else
								lastSlash = 1;
						}
						if(!lastChar)
						{
						      if(lastSlash && !comment)
								maybeWord = 1;
							else if(maybeWord)
							{
								w++;
								maybeWord = 0;
							}
							else
								w++;
						}

						if(current != 0x2F)
						{
							lastSlash = 0;
							maybeWord = 0;
						}
						lastChar = 1;
					}
				}
				if(!comment)
					c++;
		}
	}
		if(lineFlag)
			printf("%d ", l);
		if(wordFlag)
			printf("%d ", w);
		if(charFlag)
			printf("%d\n", c);
}

void wc_num()
{
	int num = 0;
	short int lastNum = 0;
	char current;
	while(current = getchar(), !feof(stdin))
	{
		if(current >= 0x30 && current <= 0x39)
			lastNum = 1;
		else
			if(lastNum)
				num++;
	}
	printf("%d", num);
}

void wc_default(short int lineFlag, short int wordFlag, short int charFlag)
{
	int c = 0, w = 0, l = 0;
	short int lastChar = 0;
	char current;
	while(current = getchar(), !feof(stdin))
	{

		switch(current)
		{
			case 0xA :		//line feed
				l++;
				c++;
				lastChar = 0;
				break;
			case 0x9:		//horizontal tab
			case 0xB:		//vertical tab
			case 0xC:		//form feed
			case 0xD:		//carriage return
			case 0x20:		//space
				lastChar = 0;
				c++;
				break;
			default :
				if(current > 0x20 && current != 0x7F)
				{
					if(!lastChar)
						w++;
					lastChar = 1;
				}
				c++;
		}
	}
	if(lineFlag)
		printf("%d ", l);
	if(wordFlag)
		printf("%d ", w);
	if(charFlag)
		printf("%d\n", c);
}