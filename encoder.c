#include <stdio.h>

short debugFlag = 1; //should debug be active
char* encryptionKey = "0"; //Encryption key for the encode function
int negative = 1; //1 if the encryption key is positive, -1 if negetive
int encIndex = 0; //Which char from encryptionKey are we on
FILE* input = 0; //set to stdin on defualt in main
FILE* output = 0; //set to stdout on defualt in main
short fileErr = 0; //was there an error openning a file stream

//Returns 1 if s1==s2 in content, 0 otherwise
int isStrEqual(char* s1, char* s2)
{
	int isEqual=1;//holds is s1 and s2 are equal 
	int isEnd = 0;//holds is atleast one string has ended
	while(isEqual==1 && isEnd==0)
	{
		if (*s1 != *s2)//inequality found
		{
			isEqual = 0;
		}
		if (*s1 == '\0' || *s2 == '\0')//stop checking if we reach end of one string
		{
			isEnd = 1;
		}
		s1+=1;
		s2+=1;

	}
	return isEqual;
}

//compares the first two chars of s1 and s2
//used to check header type
int compareHeaders(char* s1, char* s2)
{
	if(*s1==*s2 && *(s1+1)==*(s2+1))
	{
		return 1;
	}
	return 0;
}


//Sets a new input/output file
//inOut = 'i'->set input file
//inOut = 'o'->set output file
//sets fileErr flag to 1 if there was an error opening file stream
void setFile(char* fileName, char inOut)
{
	if(inOut=='i')//set input file
	{
		input = fopen(fileName,"r+");
		if(input==0)//error
		{
			fileErr = 1;
			fprintf(stderr,"Debug : error opening input file stream \"%s\". Exiting\n",fileName);
			fclose(output);
		}
	}
	else if(inOut == 'o')//set output file
	{
		output = fopen(fileName,"a+");
		if(output==0)//error
		{
			fileErr = 1;
			fprintf(stderr,"Debug : error opening output file stream \"%s\". Exiting\n",fileName);
			fclose(input);
		}
	}
	return;
}



//Encode carecter c according to the encryptionKey (with negative and encIndex included)
//loops encodindg between range of start and end (including)
//in our program it i only ranges <'0'...'9'> and <'A'...'Z'> for numbers and capital letter respectivly
char getOffset(char start, char end,char c)
{
	char offset;//c after offset
	int localKey = negative*(encryptionKey[encIndex]-'0');//amount to add/subtract from c (pre-looping) 
	offset = c+localKey;//offset char pre-looping
	while(offset>end)//make sure offset is between start and end
	{
		offset -= end-start+1;
	}
	while(offset<start)
	{
		offset += end-start+1;
	}
	return offset;
}

//advances encIndex
//makes sure it is within string limit
void advanceIndex()
{
	if(encryptionKey[encIndex+1]=='\0')//end of string
	{
		encIndex=0;
	}
	else
	{
		encIndex+=1;
	}
	return;
}


//encodes c accordingly if it is in ranges <'0'...'9'> or <'A'...'Z'>
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
	char currChar;//currenty procceced charecter
	int eof = 0;//is 0 if next charecter is not end-of-file. somthing else otherwise
	input = stdin;//in past tense it is "stded"
	output = stdout;

	for (int i=1;i<argc;i++)//go over run arguments
	{
		if (debugFlag==1)//print argvs if debug is on
		{
			fprintf(stderr,"%s\n",argv[i]);
		}

		if(isStrEqual(argv[i],"-d")==1)// check if thee is a "-d" flag to turn debug off
		{
			debugFlag=0;
		}

		else if(isStrEqual(argv[i],"+d")==1)// check if thee is a "+d" flag to turn debug on
		{
			debugFlag=1;
		}
		else if (compareHeaders(argv[i],"-e")==1)//check if flag is a negative encryption key
		{
			encryptionKey=argv[i]+2;
			negative =-1;
		}
		else if (compareHeaders(argv[i],"+e")==1)//check if flag is a positive encryption flag
		{
			encryptionKey=argv[i]+2;
		}
		else if (compareHeaders(argv[i],"-i")&&fileErr==0)//check if flag to change input file
		{
			setFile(argv[i]+2,'i');
		}
		else if (compareHeaders(argv[i],"-o")&&fileErr==0)//check if flag to change output file
		{
			setFile(argv[i]+2,'o');
		}
	}

	if (fileErr==0)//proceed if there were no errors when opening files
	{

		eof = feof(input);
		while(eof==0)
		{
			currChar = fgetc(input);//get next char
			eof=feof(input);//check if were at end of file
			if(eof==0)//if not
			{
				if (debugFlag==1)
				{
					fprintf(stderr,"\nDEBUG : adding %c\n",currChar);//print unencoded char
				}
				fputc(encode(currChar),output);//add encoded char to output file
				advanceIndex();//addvance enc index
			}
		}
		if (debugFlag==1)
		{
			fprintf(stderr,"\nDEBUG : End Of File:)\n");//mark that file ended
		}
		fclose(input);//clode input
		fclose(output);//close output
		return 0;
	}
}
