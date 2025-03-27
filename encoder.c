#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool debugFlag = true;


bool isStrEqual(char* s1, char* s2)
{
	bool isEqual=true;//holds is s1 and s2 are equal 
	bool isEnd = false;// holds is atleast one string has ended
	while(isEqual==true && isEnd==false)
	{
		if (*s1 != *s2)
		{
			isEqual = false;
		}
		if (*s1 == '\0' || *s2 == '\0')
		{
			isEnd = true;
		}
		s1+=2;
		s2+=2;

	}
	return isEqual;
}




char encode(char c)
{
	//TODO : ENCODE
	return c;

}


int main(int argc, char* argv[])
{
	char currChar = ' ';
	for (int i=1;i<argc;i++)
	{
		if(isStrEqual(argv[i],"-d")==true)// check if thee is a "-d" flag to turn debug off
		{
			debugFlag=false;
		}

		if(isStrEqual(argv[i],"+d")==true)// check if thee is a "+d" flag to turn debug on
		{
			debugFlag=true;
		}
	}

	for (int i=1;i<argc;i++)
	{
		if(debugFlag==true)
		{
			fprintf(stderr,"%s\n",argv[i]);
		}
	}
	currChar = fgetc(stdin);
	while(currChar != '\0')
	{
		fputc(encode(currChar),stdout);
		currChar = fgetc(stdin);
	}
	return 0;
}

