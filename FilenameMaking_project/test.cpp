// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main(int argc, char* argv[])
{	
	FILE * fp = fopen("face.txt","w");
	char str[100];
	for(int i=1; i<1000; ++i)
	{
		sprintf(str,"./frontal/frontal%d.jpg 1 0 0 19 19\n",i);		
		fprintf(fp,"%s",str);
	}
	fclose(fp);

	return 0;
}

