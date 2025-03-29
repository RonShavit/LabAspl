#include <stdio.h>
#include <string.h>

short debugFlag = 1; //sould debug be active
char* encryptionKey = "0"; //Encryption key for the encode function
int negative = 1;
int encIndex = 0;
FILE* input = 0; //set to stdin on defualt in main
FILE* output = 0; //set to stdout on defualt in main
short fileErr = 0; //was there an error openning a file stream

//Returns 1 if s1==s2 in content, 0 otherwise
int isStrEqual(char* s1, char* s2)
{
	int isEqual=1;//holds is s1 and s2 are equal 
	int isEnd = 0;// holds is atleast one string has ended
	while(isEqual==1 && isEnd==0)
	{
		if (*s1 != *s2)
		{
			isEqual = 0;
		}
		if (*s1 == '\0' || *s2 == '\0')
		{
			isEnd = 1;
		}
		s1+=1;
		s2+=1;

	}
	return isEqual;
}

int compareHeaders(char* s1, char* s2)
{
	if(*s1==*s2 && *(s1+1)==*(s2+1))
	{
		return 1;
	}
	return 0;
}


void setFile(char* fileName, char inOut)
{
	if(inOut=='i')
	{
		input = fopen(fileName,"r+");
		if(input==0)
		{
			fileErr = 1;
			fprintf(stderr,"Debug : error opening input file stream");
			fclose(output);
		}
	}
	else if(inOut == 'o')
	{
		output = fopen(fileName,"a+");
		if(output==0)
		{
			fileErr = 1;
			fprintf(stderr,"Debug : error opening output file stream");
			fclose(input);
		}
	}
	return;
}




char getOffset(char start, char end,char c)
{
	char offset;
	int localKey = negative*(encryptionKey[encIndex]-'0');
	offset = c+localKey;
	while(offset>end)
	{
		offset -= end-start+1;
	}
	while(offset<start)
	{
		offset += end-start+1;
	}
	return offset;
}

void advanceIndex()
{
	if(encryptionKey[encIndex+1]=='\0')
	{
		encIndex=0;
	}
	else
	{
		encIndex+=1;
	}
	return;
}


char encode(char c)
{

	if (c<='9'&&c>='0')
	{
		c = getOffset('0','9',c);
	}
	else if(c<='Z'&&c>='A')
	{
		c=getOffset('A','Z',c);
	}
	return c;

}


int main(int argc, char* argv[])
{
	int eof = 0;
	input = stdin;
	output = stdout;
	char currChar = ' ';
	for (int i=1;i<argc;i++)
	{
		if(isStrEqual(argv[i],"-d")==1)// check if thee is a "-d" flag to turn debug off
		{
			debugFlag=0;
		}

		else if(isStrEqual(argv[i],"+d")==1)// check if thee is a "+d" flag to turn debug on
		{
			debugFlag=1;
		}
		else if (compareHeaders(argv[i],"-e")==1)
		{
			encryptionKey=argv[i]+2;
			negative =-1;
		}
		else if (compareHeaders(argv[i],"+e")==1)
		{
			encryptionKey=argv[i]+2;
		}
		else if (compareHeaders(argv[i],"-i")&&fileErr==0)
		{
			setFile(argv[i]+2,'i');
		}
		else if (compareHeaders(argv[i],"-o")&&fileErr==0)
		{
			setFile(argv[i]+2,'o');
		}
	}

	for (int i=1;i<argc;i++)
	{
		if(debugFlag==1)
		{
			fprintf(stderr,"%s\n",argv[i]);
		}
	}
	if (fileErr==0)
	{

		eof = feof(input);
		while(eof==0)
		{
			currChar = fgetc(input);
			eof=feof(input);
			if(eof==0)
			{
				if (debugFlag==1)
				{
					fprintf(stderr,"\nDEBUG : adding %c\n",currChar);
				}
				fputc(encode(currChar),output);
				advanceIndex();
			}
		}
		if (debugFlag==1)
		{
			fprintf(stderr,"\nDEBUG : End Of File:)\n");
		}
		fclose(input);
		fclose(output);
		return 0;
	}
}

