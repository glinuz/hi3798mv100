#include <stdio.h>
#include <unistd.h>

int main(int argc ,char* argv[])
{
	int test_num = atoi(argv[2]);
	char  aData[15] = {0};
	FILE *fp;     
	
	if( ( test_num < 1 ) || ( test_num > 15 ) )
	{
		printf("fail num=%d , num must 1~15 \n",test_num);
		return -1 ;
	}

	if ( access(argv[1], F_OK) )
	{
		printf("error , cann't find \"%s\"\n",argv[1]);
		return -1 ;
	}

	fp=fopen(argv[1],"ab+");
	
	if(!fp || (test_num > 16))   
	{
		printf("open %s fail\n",argv[1]);
		return -1 ;
	}   
	fwrite(&aData, 1, test_num*sizeof(char), fp); 
	fclose(fp);   
	return 0;
}
