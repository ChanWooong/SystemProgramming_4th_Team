내 드라이브
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
    int *data;
    char buffer[MAX] = {0};
    fread(buffer, 1, MAX, fd);

    printf("---------------- origin ----------------\n\n");
    printf("%s", buffer);
 
    int n = strlen(find);
    int m = strlen(buffer);
    printf("\n\n---------------- fine ----------------\n\n");
    data = KMP(buffer, find, m, n);
    int point;
    
    for(int k = 0; k < m; k++){
	if(k == data[point]){
		printf("[");	
	}
	if(k == data[point]+n){
		printf("]");
		k--;
		point++;
		continue;
	}
	printf("%c",buffer[k]);
    }
    
    printf("\n");
    
 
}
