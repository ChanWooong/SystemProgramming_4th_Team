#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "grepLib.h"
#include <curses.h>
#include <signal.h>
#define MAX 3000



//KMP의 리턴값은 시작포인트 int배열, 문자열이 없는 경우 NULL 출력
void init(){
	wrefresh(win);
	wrefresh(title);
	wrefresh(win2);
    wrefresh(content);
}
int n_q(){
	wrefresh(content);
        noecho();
	int ch = wgetch(win2);
	if(tolower(ch) == 'n') {
		clear();
		return 1;
	}
	if(tolower(ch) == 'q') {
		clear();
		//sleep(2);
		return 0;
	}
}

char* Strlwr(char* string){ //option_i에 필요(대소문자 구별 X)
    int i = 0;
    char* result_string = (char*)malloc(sizeof(char) * strlen(string));
    //printf("%s", string);
    for(int k = 0; k<strlen(string);k++){
        result_string[k] = tolower(string[k]);
    }
    return result_string;
}


// -l : 문자열이 일치한 파일의 이름만 출력한다.
// void option_l(char* filename, char* findstr, int find_length){
//     FILE *fp;

// }

// -r : 서브 디렉터리의 파일까지 모두 출력한다.
/*
void option_r(char *dir){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char buff[512];
    int i = 0;
    char* filelist[128];
    char* findstr;

    if((dp = opendir(dir)) == NULL){
        return;
    }

    while((entry = readdir(dp)) != NULL){
        stat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            if(!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name)){
                continue;
            }
            sprintf(buff, "%s/%s", dir, entry->d_name);
            chdir(buff);
            subdir(buff);
        }
        else{
            sprintf(buff, "%s/%s", dir, entry->d_name);
            option_none(findstr, entry->d_name, find_length);
            printf("%s\n", buff);

        }
    }
}
*/
void option_none(char* filename, char* findstr, int find_length){
    char** buffer = getdata(filename);
    int buffer_size = getbuffersize(data_buffer);
    int line_length, *data;
    for(int i = 0; i<buffer_size; i++){
	    init(); 
        line_length = strlen(buffer[i]);
        data = KMP(buffer[i], findstr, line_length, find_length);
        if(data != NULL){
            print_threeline(i, buffer, data, find_length, 5);
        }
        else{
            continue;
        }
        if(n_q() == 0) break;
        else continue;
    }
    clear();
}

void option_all(FILE* fp, char* findstr, int find_length){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char buff[256];

    if((dp = opendir(".")) == NULL){//현재 디렉토리로 진입
        return;
    }

    while((entry = readdir(dp)) != NULL){
        stat(entry->d_name, &statbuf);
        if(!S_ISDIR(statbuf.st_mode)){
            option_none(entry->d_name, findstr, find_length);
        }    
    }
    closedir(dp);
}


void option_l(char* findstr, int find_length){
	
	struct dirent *entry = NULL;
	int i = 0;
	DIR *d = NULL;
	d = opendir(".");
	init();
	int y = 4;
	char ch;
	while(1){
		if((entry = readdir(d)) == NULL){
			wmove(content, ++y, 2);
			waddstr(content,"the file list is end");
			clear();
			break;		
		}
		char *name;
		char buffer[MAX] = {0};
		FILE* fd = fopen(entry->d_name, "r");
		fread(buffer,1, MAX, fd);
		int* data = NULL;
		data = KMP(buffer, findstr ,strlen(buffer), strlen(findstr));
		wmove(content,2,2);
		waddstr(content,"*match pattern File list*");
		wmove(content, 3,2);
		if(data != NULL){
			wmove(content,y++,2);
    			waddstr(content,entry->d_name);
    	
		}
		if(n_q() == 0) break;
		else continue;
	}
}

// -c : 문자열이 포함된 라인 개수를 표시한다.
void option_c(char* filename, char* findstr, int find_length){
    FILE *fp;
    int cnt = 0, line_length;
    char* line;
    char buffer[256];
    fp = fopen(filename, "r");
    while(fgets(buffer, 256, fp)!=NULL){
        if(buffer == NULL)
            break;
        line_length = strlen(buffer);
        line = buffer;
        
        if(strstr(buffer, findstr) != NULL){
        //KMP의 리턴값이 NULL이 아니면, cnt++;
            cnt++;
        }
        line = NULL;
    }
    printf("<%d>", cnt);
    //printf("line that have "%s" : %d\n", find, string);
    fclose(fp);
}

// -i : 문자열의 대소문자를 구분하지 않는다.
void option_i(char* filename, char* findstr, int find_length){
    char** buffer = getdata(filename);
    char* lwr_line, *lwr_find;
    int buffer_size = getbuffersize(data_buffer);
    int line_length, *data;

    for(int i = 0; i<buffer_size; i++){
    	init();
    	line_length = strlen(buffer[i]);
        lwr_line = Strlwr(buffer[i]);
        lwr_find = Strlwr(findstr);
        data = KMP(lwr_line, lwr_find, line_length, find_length);
        if(data != NULL){
            print_threeline(i, buffer, data, find_length, 5);
        }
        else{
            continue;
        } 
        if(n_q() == 0) break;
        else continue;
    }
    clear();
}

// -h : 파일 이름을 출력하지 않는다.
void option_h(char* filename, char* findstr, int find_length){
    char** buffer = getdata(filename);
    int buffer_size = getbuffersize(data_buffer);
    int line_length, *data;

    for(int i = 0; i<buffer_size; i++){
        line_length = strlen(buffer[i]);
        data = KMP(buffer[i], findstr, line_length, find_length);
        if(data != NULL){
            print_threeline(i, buffer, data, find_length, 8);
        }
        else{
            continue;
        }
    }
}

// -v : 입력한 패턴이 포함되지 않은 문자열을 출력한다.
void option_v(char* filename, char* findstr, int find_length){
    char** buffer = getdata(filename);
    int buffer_size = getbuffersize(data_buffer);
    int line_length, *data;
    for(int i = 0; i < buffer_size; i++){
        init();
        line_length = strlen(buffer[i]);
        data = KMP(buffer[i], findstr, line_length, find_length);
        if(data != NULL){
            continue;
        }
        else{
            print_threeline(i, buffer, data, find_length, 8);
        }
        if(n_q() == 0) break;
        else continue;
    }
    clear();
}

// -n : 일치한 문자열이 포함된 라인 번호를 출력한다.
void option_n(char* filename, char* findstr, int find_length){
    char** buffer = getdata(filename);
    int buffer_size = getbuffersize(data_buffer);
    int line_length, *data;
    char find_linum[50][100];
    int index=0;
    init();
    for(int i = 0; i<buffer_size; i++){
        line_length = strlen(buffer[i]);
        data = KMP(buffer[i], findstr, line_length, find_length);
        if(data != NULL){
            sprintf(find_linum[index], "%d", i+1);
            index++;
        }
    }
    wmove(content,2,2);
    waddstr(content, "the file name : ");
    waddstr(content, filename);
    wmove(content,3,2);
    waddstr(content, "the line number of pattern");
    int x = 2,y = 4;
    for(int k = 0; k<index; k++){
        wmove(content,y++,2);
    	wstandout(content);
    	waddstr(content,find_linum[k]);
    	wstandend(content);
    	
    	if(n_q() == 0) break; 	
    }
}
// -w : 입력한 문자열이 독립된 단어로 존재하는 경우만 출력한다.
void option_w(char* filename, char* findstr, int find_length){
    char** buffer = getdata(filename);
    int buffer_size = getbuffersize(data_buffer);
    int line_length, *data;
    int k, l;
    char last[256];
    for(int i = 0; i < buffer_size; i++){
        line_length = strlen(buffer[i]);
        data = KMP(buffer[i], findstr, line_length, find_length);
        if(data != NULL){
            int data_size = getarraysize(data);
            for(int j = 0; j < data_size; j++){
                k = data[j] + find_length;
                if(data[j] != 0){
                    l = data[j] -1;
                    if((buffer[i][k] < 65 || buffer[i][k] > 90) && (buffer[i][k] < 97 || buffer[i][k] > 122)){   
                        if((buffer[i][l] < 65 || buffer[i][l] > 90) && (buffer[i][l] < 97 || buffer[i][l] > 122))
                            print_threeline(i, buffer, data, find_length, 5);
                        break;
                    }
                }
                else{
                    if((buffer[i][k] < 65 || buffer[i][k] > 90) && (buffer[i][k] < 97 || buffer[i][k] > 122)){   
                        print_threeline(i, buffer, data, find_length, 5);
                        break;
                    }
                }
            }
        }
        else{
            continue;
        }
        if(n_q() == 0) 
            break;
        else
            continue;
    }
}
