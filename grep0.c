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
#define COLOR_DEFAULT "\033[49m"
#define COLOR_ORANGE "\033[43m"


//출력에 필요한 함수와 전역변수
char* data_buffer[100];
char** getdata(char* filename);
int getbuffersize(char** buffer);
int getarraysize(int* arr);
void printline(char* line, int* start_index, int find_length, int linenum);
void print_threeline(int i, char** buffer, int* data_KMP, int find_len, int linenum);
#define blankstr "                             "


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


void displayScreen(FILE* fd, char* find, char* filename);
void option_l(char* findstr, int find_length);
void restore(int fd, struct termios* prev_term);
int getch();

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
    		displayScreen(fd,find,argv[2]);
    	}
    }
    else if(argc == 4){
    		
    	if(argv[1][0] == '-'){
    		option = argv[1][1];	
    		if(option == 'c'){
    			strcpy(find, argv[2]);
			option_c(argv[3], find,strlen(find));
		}
	}
    }
    else if(argc == 6 && argv[1][1] == 'p'){
    	
    }
    return 0;
}
void displayScreen(FILE* fd, char* find, char* filename){
    int *data;
    char buffer[MAX] = {0};
    char cur_line[MAX] = {0};
    char pre_line[BUFFERSIZ][MAX] = {0};
    int index = 0;
    int buf_line = 0;
    bool found = false;
    bool skip = false;
    int input = false;
    
    
    fread(buffer, 1, MAX, fd);
    int n = strlen(find);
    int m = strlen(buffer);
    fclose(fd);
    
    data = KMP(buffer, find, m, n);
    printf("[%d]",data[0]);
   if(data != NULL){
   	fd = fopen(filename, "r");
   	setup();
	
    	while (fgets(cur_line, MAX, fd) != NULL)
    	{	
    		int *l_data;
			clear();

        	if (strstr(cur_line, find) != NULL)
        	{
        	    found = true;
		    	skip = true;

        	    int n = strlen(find);
        	    int m = strlen(cur_line);

	  	    	printf("\n\n\n\n");
			
		    	int pre_index = (index-1+BUFFERSIZ)%BUFFERSIZ;
		    	if(strlen(pre_line[pre_index]) > 0){
		    		printf("%s\n", pre_line[pre_index]);
		    	}

	  	    	l_data = KMP(cur_line, find, m, n);
	  	    
	  	    	if(l_data != NULL){
	  	    		int cnt = 0;
    		   		for(int k = 0; k < m; k++){
						if(k == l_data[cnt]){
						printf(COLOR_ORANGE);	
						}
						if(k == l_data[cnt]+n){
							printf(COLOR_DEFAULT);
							cnt++;
							k--;
							continue;
						}
						printf("%c",cur_line[k]);
    		    	}	
    		    	printf(COLOR_DEFAULT);
	  			}
		    	//printf("%s", cur_line);	    

   			}
    	   	else {
				strncpy(pre_line[index], cur_line, MAX);
            	index = (index + 1) % BUFFERSIZ;
	    		skip = true;
    	  	}
    
    		if(flag){
	  	 		break;
    	  	}
		       
	  		if(!flag) {
				while(1) {
					printf("\n\nn : next q : quit\n\n\n");
					int ch = getch();
					if(tolower(ch) == 'n')
						break;
					else if(tolower(ch) == 'q') {
						flag = 1;
						break;
					}
					else if(!isspace(ch)) {
						printf("n : next q : quit\n\n\n");
					}
				}
				input = false;
    	   }
		}	
		if (!found) {
      	 	printf("Cannot find pattern\n");
   		}
    	fclose(fd);
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


char** getdata(char* filename){//파일을 열어 char*배열에 파일 내용 넣어 반환
	FILE* fp;

	for(int i = 0; i < 100; i++){
		data_buffer[100] = NULL;
	}
    char* line_buffer;
    int buffer_i = 0;

    fp = fopen(filename, "r");

    while(1){
        line_buffer = (char*)malloc(sizeof(char) * 256);
        data_buffer[buffer_i] = fgets(line_buffer, 256, fp);
        if(data_buffer[buffer_i] == NULL)
            break;
        buffer_i++;
    }
    buffer_i = 0;

	return data_buffer;
}

int getbuffersize(char** buffer){//전체 파일을 줄의 갯수 반환
    int i=0, size = 0;
    while(buffer[i++]!=NULL){
        size++;
    }
    return size;
}

int getarraysize(int* arr){//KMP의 반환값의 크기 반환
    int i = 0, size = 0;
    if(arr[0] == 0){
        size = 1;
        if(arr[1] != 0){
            while(arr[++i] != 0)
                size++;
        }
    }
    else{
        while(arr[i++] != 0)
            size++;
    }
    return size;
}

void printline(char* line, int* data_KMP, int find_length, int linenum){//문자열 탐색 결과 출력
    int arrsize = getarraysize(data_KMP);

    for(int i = 0 ;i < strlen(line); i++){
        move(linenum, i + 10);
        for(int j = 0; j < arrsize; j++){
            if(i == data_KMP[j]){
                standout();
            }
            else if(i == data_KMP[j] + find_length){
                standend();
            }
        }
        addch(line[i]);
    }
    refresh();
}

void print_threeline(int i, char** buffer, int* data_KMP, int find_length, int linenum){//기본 출력 형태
    int buffersize = getbuffersize(buffer);
    int arrsize = getarraysize(data_KMP);
    if(i == 0){
        printline(buffer[i], data_KMP, find_length, linenum);
        move(linenum+2, 10);
        addstr(buffer[i+1]);
    }
    else if(i == buffersize -1){
        move(linenum - 2, 10);
        addstr(buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
        move(linenum+2, 10);
        addstr(blankstr);

    }
    else{
        move(linenum - 2, 10);
        addstr(buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
        move(linenum+2, 10);
        addstr(buffer[i+1]);
    }
    refresh();
    sleep(1);
}