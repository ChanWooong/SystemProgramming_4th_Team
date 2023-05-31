#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <curses.h>
#include <termios.h>

#define MAX 3000
#define blankstr "                               "


//출력에 필요한 함수와 전역변수
WINDOW *win;
WINDOW *win2;
WINDOW *title;
WINDOW *content;
WINDOW *border_line;
WINDOW *file_name;

char* data_buffer[512];
void makeUI();
char** getdata(char* filename);
int getbuffersize(char** buffer);
int getarraysize(int* arr);
void printline(char* line, int* start_index, int find_length, int linenum);
void print_fiveline(int i, char** buffer, int* data_KMP, int find_len, int linenum);
void restore(int fd, struct termios* prev_term);
void myhandler(int sig){
	signal(sig, SIG_IGN);
}

int main(int argc, char* argv[])
{   
    char findstr[MAX], filename[64];
    char option;
    
	signal(SIGINT, myhandler);
	signal(SIGQUIT, myhandler);

     if(argc == 2){
       makeUI();
       basicGrep(argv[1]);
       endwin();
    }
    
    else if(argc == 3){
    	if(argv[1][0] == '-'){ // 옵션이 있는 경우
			strcpy(findstr, argv[2]);
    		option = argv[1][1];
    		if(option == 'l'){
    			makeUI();
    			option_l(findstr, strlen(findstr));
  			}
			if(option == 'a'){ // *과 같은 기능
    			makeUI();
    			option_all(findstr, strlen(findstr));	
  			}
			if(option == 'r'){ //서브디렉토리까지 모두 탐색
    			makeUI();
    			option_r(findstr, strlen(findstr));	
  			}   
    	}
    	else { // 옵션이 없는 경우
			strcpy(findstr, argv[1]);
			strcpy(filename, argv[2]);
    		makeUI();
    		option_none(filename, findstr,strlen(findstr)); // none option
    	}
        endwin();
    }
    else if(argc == 4){
		strcpy(findstr, argv[2]);
		strcpy(filename, argv[3]);
    	if(argv[1][0] == '-'){ // 옵션이 있는 경우
    		option = argv[1][1];	
    		if(option == 'c'){
				makeUI();
				option_c(filename, findstr,strlen(findstr)); // c option
			}
			if(option == 'i'){
				makeUI();
				option_i(filename, findstr,strlen(findstr)); // i option
			}
			if(option == 'n'){
				makeUI();
				option_n(filename, findstr,strlen(findstr)); // n option
			}
            if(option == 'v'){
				makeUI();
				option_v(filename, findstr ,strlen(findstr)); // v option
			}
			if(option == 'w'){
				makeUI();
				option_w(filename, findstr,strlen(findstr)); // w option
			}
			if(option == 'c'){
				makeUI();
				option_c(filename, findstr,strlen(findstr)); // c option
			}
		}
        endwin();
    }
    else if(argc == 5 && argv[1][1] == 'p'){
		pipeLine(argv[2], argv[4]);
    }

    return 0;
}
void makeUI(){ //UI를 window로 구현.
	initscr();
	clear();
	win = newwin(25,90,2,5);
	box(win, ACS_VLINE, ACS_HLINE);

	file_name = subwin(win, 2, 15, 5, 10);
	wmove(file_name, 30, 1);
	waddstr(file_name, "File:");
	
	title = subwin(win,3,20,3,39);
	box(title, ACS_VLINE, ACS_HLINE);
	wmove(title,1,4);
	waddstr(title,"find pattern");
	
	win2 = subwin(win,5, 86, 21, 7);
	box(win2, ACS_VLINE, ACS_HLINE);
	wmove(win2,2,22);
	waddstr(win2,"Please press (next : 'n' | quit : 'q')");
	
	content = subwin(win,13,84, 7,8);
	
	border_line = subwin(win,15, 86, 6,7);
	box(border_line, ACS_VLINE, ACS_HLINE);

	

	wmove(content, 2,2);
	wrefresh(win);
	wrefresh(title);
	wrefresh(win2);
	wrefresh(content);
	wrefresh(border_line);
    wrefresh(file_name);
    
	
    // start_color();
    // init_pair(1, COLOR_BLUE, COLOR_BLACK);
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
	if(arr != NULL){
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
	}
	else{
		return 0;
	}
    
    return size;
}

void printline(char* line, int* data_KMP, int find_length, int linenum){//문자열 탐색 결과 출력
    int arrsize = getarraysize(data_KMP);

    if(data_KMP != NULL){
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
	else{
		wmove(content, linenum, 2);
		//wattron(content, COLOR_PAIR(1));
		waddstr(content, line);
		//wattroff(content, COLOR_PAIR(1));
	}
}

void print_fiveline(int i, char** buffer, int* data_KMP, int find_length, int linenum){//기본 출력 형태
    int buffersize = getbuffersize(buffer);
    int arrsize = getarraysize(data_KMP);
    if(i == 0){
        printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,buffer[i+1]);
		wmove(content, linenum+3, 2);
		waddstr(content, buffer[i+2]);
    }
	else if(i == 1){
		wmove(content,linenum-2, 2);
        waddstr(content,buffer[i+1]);
		printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,buffer[i+1]);
		wmove(content, linenum+3, 2);
		waddstr(content, buffer[i+2]);
	}
    else if(i == buffersize -2){
		wmove(content,linenum - 3, 2);
        waddstr(content,buffer[i-2]);
        wmove(content,linenum - 2, 2);
        waddstr(content,buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
		wmove(content,linenum + 2, 2);
        waddstr(content,buffer[i-1]);
        wmove(content,linenum+3, 2);
        waddstr(content,blankstr);

    }
	else if(i == buffersize -3){
        wmove(content,linenum - 2, 2);
        waddstr(content,buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,blankstr);
		wmove(content,linenum+3, 2);
        waddstr(content,blankstr);

    }
    else{
		wmove(content,linenum - 3, 2);
        waddstr(content,buffer[i-2]);
        wmove(content,linenum - 2, 2);
        waddstr(content,buffer[i-1]);
        printline(buffer[i], data_KMP, find_length, linenum);
        wmove(content,linenum+2, 2);
        waddstr(content,buffer[i+1]);
		wmove(content,linenum+3, 2);
        waddstr(content,buffer[i+2]);
    }
}

