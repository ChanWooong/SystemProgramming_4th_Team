#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"
#include <unistd.h>
#include <fcntl.h>
#define MAX 3000
void displayScreen(FILE* fd, char* find);

int main(int argc, char* argv[])
{   
    FILE* fd;
    char find[MAX];
    char option;
    
    if(argc < 3){
    	printf("check the number of parameters.\n");
	exit(1);
    }
    if(argc == 3){
    	fd = fopen(argv[2], "r");
    	strcpy(find,argv[1]);
    	displayScreen(fd, find);
    }
    if(argc == 4){
    	option = argv[1][1];
    	fd = fopen(argv[3], "r");
    	strcpy(find,argv[2]);
    	displayScreen(fd, find);
    }

    return 0;
}

void displayScreen(FILE* fd, char* find){
    int data[MAX] = {0};
    char buffer[MAX] = {0};
    fread(buffer, 1, MAX, fd);

    printf("---------------- origin ----------------\n\n");
    printf("%s", buffer);
 
    int n = strlen(buffer);
    int m = strlen(find);
    printf("\n\n---------------- fine ----------------\n\n");
    data = KMP(buffer, find, n, m);
    printf("\n");
    for(int i = 0; i < sizeof(data)/4; i++){
    	printf("[%d]", data[i]);
    }
 
}

