#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
 
#define MAX 3000
void displayScreen(FILE* fd, char* find);
void option_l(char* findstr, int find_length);

int main(int argc, char* argv[])
{   
    FILE* fd;
    char find[MAX];
    char option;
    
    if(argc < 3){
    	printf("check the number of parameters.\n");
	exit(1);
    }
    
    else if(argc == 3){
        printf("%c",argv[1][0]);
    	if(argv[1][0] == '-'){
    		option = argv[1][1];
    		if(option == 'l'){
    			strcpy(find,argv[2]);
    			option_l(find, strlen(find));
  		}
    	}
    	else {
    		fd = fopen(argv[2], "r");
    		strcpy(find,argv[1]);
    		displayScreen(fd, find);
    	}
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
    printf("\n\n---------------- find ----------------\n\n");
    data = KMP(buffer, find, m, n);
   	if(data != NULL){
        int point = 0;
    
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
    
 
}
void option_l(char* findstr, int find_length){
	
	struct dirent *entry = NULL;
	int i = 0;
	DIR *d = NULL;
	d = opendir(".");
	printf("---------------File List-------------\n");
	while((entry = readdir(d))!=NULL){
		char name[100];
		char buffer[MAX] = {0};
		FILE* fd = fopen(entry->d_name, "r");
		fread(buffer,1, MAX, fd);
		int* data = NULL;
		data = KMP(buffer, findstr ,strlen(buffer), strlen(findstr));
		if(data != NULL)
			printf("%s\n",entry->d_name);
	/*
		for(int i = 0 ; i<3; i++){
			printf("%d, ",data[i]);
		}
		printf("\n");
	*/	

	}
	printf("--------------------------------------\n");
}


