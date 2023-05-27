#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdbool.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>
 
#define BUFFERSIZ 3
#define MAX 3000

#ifdef _WIN32
void clear() {
	system("cls");
}
#else
void clear() {
	printf("\033[H\033[J");
}
#endif

volatile sig_atomic_t flag = 0;

void signal_handler(int signal) {
	if(signal == SIGINT)
		flag = 1;
}


void setup() {
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

void set_mode(int fd, struct termios* prev_term) {
	struct termios term;
	tcgetattr(fd, prev_term);
	term = *prev_term;
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(fd, TCSANOW, &term);
}


void displayScreen(FILE* fd, char* find);
void option_l(char* findstr, int find_length);
void restore(int fd, struct termios* prev_term);
int getch();

int main(int argc, char* argv[])
{   
    FILE* fd;
    char find[MAX];
    char option;
    char cur_line[MAX] = {0};
    char pre_line[BUFFERSIZ][MAX] = {0};
    int index = 0;
    int buf_line = 0;
    bool found = false;
    bool skip = false;
    
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

void restore(int fd, struct termios* prev_term) {
	tcsetattr(fd, TCSANOW, prev_term);
}

int getch() {
	int ch;
	struct termios old_term;
	set_mode(STDIN_FILENO, &old_term);
	ch = getchar();
	restore(STDIN_FILENO, &old_term);
	return ch;
}
