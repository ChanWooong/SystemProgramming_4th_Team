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
#include <curses.h>
#include <termios.h>

 
#define BUFFERSIZ 3
#define MAX 3000
#define COLOR_DEFAULT "\033[49m"
#define COLOR_ORANGE "\033[43m"
#define blankstr "                               "


//출력에 필요한 함수와 전역변수
WINDOW *win;
WINDOW *win2;
WINDOW *title;
WINDOW *content;

char* data_buffer[100];
char** getdata(char* filename);
int getbuffersize(char** buffer);
int getarraysize(int* arr);
void makeUI();
void printline(char* line, int* start_index, int find_length, int linenum);
void print_threeline(int i, char** buffer, int* data_KMP, int find_len, int linenum);

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


//void displayScreen(FILE* fd, char* find, char* filename);
void option_l(char* findstr, int find_length);
void restore(int fd, struct termios* prev_term);

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
    		makeUI();
    		option_none(argv[2],argv[1],strlen(argv[1])); // none option 케이스 호출
			endwin();
    		//displayScreen(fd,find,argv[2]);
    	}
    }
    else if(argc == 4){
    		
    	if(argv[1][0] == '-'){
    		option = argv[1][1];	
    		if(option == 'c'){
    			strcpy(find, argv[2]);
				option_c(argv[3], find,strlen(find));
			}
			if(option == 'i'){
				makeUI();
				option_i(argv[3],argv[2],strlen(argv[2]));
			}
		}
    }
    else if(argc == 6 && argv[1][1] == 'p'){
    	
    }
    return 0;
}
void makeUI(){ //UI를 window로 구현.
	initscr();
	clear();
	win = newwin(23,60,2,7);
	box(win, ACS_VLINE, ACS_HLINE);
	
	title = subwin(win,3,20,3,27);
	box(title, ACS_VLINE, ACS_HLINE);
	wmove(title,1,4);
	waddstr(title,"find pattern");
	
	win2 = subwin(win,5,58,19,8);
	box(win2, ACS_VLINE, ACS_HLINE);
	wmove(win2,2,10);
	waddstr(win2,"Please press (next : 'n' | quit : 'q')");
	
	content = subwin(win,13,58,6,8);
	box(content, ACS_VLINE, ACS_HLINE);
	wmove(content, 2,2);
	wrefresh(win);
	wrefresh(title);
	wrefresh(win2);
	wrefresh(content);

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
        wmove(content, linenum, i + 2);
        for(int j = 0; j < arrsize; j++){
            if(i == data_KMP[j]){
                wstandout(content);
            }
            else if(i == data_KMP[j] + find_length){
                wstandend(content);
            }
        }
        waddch(content,line[i]);
    }
}

void print_threeline(int i, char** buffer, int* data_KMP, int find_length, int linenum){//기본 출력 형태
    int buffersize = getbuffersize(buffer);
    int arrsize = getarraysize(data_KMP);
    if(i == 0){
        printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,buffer[i+1]);
    }
    else if(i == buffersize -2){
        wmove(content,linenum - 2, 2);
        waddstr(content,buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,blankstr);

    }
    else{
        wmove(content,linenum - 2, 2);
        waddstr(content,buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,buffer[i+1]);
    }
}

