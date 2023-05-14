#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"
#include <unistd.h>
#include <fcntl.h>
#define MAX 3000

int main(int argc, char* argv[])
{   
    FILE* fd;
    char buffer[MAX] = {0};
    char find[MAX];
    
    if(argc != 3){
    	printf("check the number of parameters.\n");
	exit(1);
    }
    fd = fopen(argv[2], "r");
    
    fread(buffer, 1, MAX, fd);
    strcpy(find,argv[1]);
    printf("---------------- origin ----------------\n\n");
    printf("%s", buffer);
 
    int n = strlen(buffer);
    int m = strlen(find);
    printf("\n\n---------------- fine ----------------\n\n");
    KMP(buffer, find, n, m);
 
    return 0;
}

