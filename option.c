#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>
#include "grepLib.h"

//KMP의 리턴값은 시작포인트 int배열, 문자열이 없는 경우 NULL 출력

void Strlwr(char string[]){ //option_i에 필요(대소문자 구별 X)
    int i = 0;
    while(string[i] != '\0'){
        string[i] = (char)tolower(string[i]);
        i++;
    }
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
    FILE *fp;
    int cnt = 0, line_length;
    char* line, buffer[256];

    fp = fopen(filename, "r");
    while(1){
        line = fgets(buffer, 256, fp);
        if(line == NULL)
            break;
        line_length = strlen(line);
        if(KMP(buffer, findstr, line_length, find_length) != NULL){//KMP의 리턴값이 NULL이 아니면 출력
            //출력
        }
    }
    fclose(fp);
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
    FILE *fp;
    char* line, buffer[256];
    int line_length;
    
    fp = fopen(filename, "r");

    while(1){
        line = fgets(buffer, 256, fp);
        if(line == NULL)
            break;
        line_length = strlen(line);
        Strlwr(buffer);
        Strlwr(findstr);
        if(KMP(findstr, buffer, line_length, find_length) != NULL){//KMP의 리턴값이 NULL이 아니면, cnt++;
            
        }
    }
    fclose(fp);
}

// -h : 파일 이름을 출력하지 않는다.
void option_h(char* filename, char* findstr, int find_length){
    FILE *fp;
    char* line, buffer[256];
    int line_length;

    fp = fopen(filename, "r");

    while(1){
        line = fgets(buffer, 256, fp);
        line_length = strlen(line);
        if(line == NULL){
            break;
        }
        if(KMP(findstr, line, line_length, find_length) != NULL){
            //출력

        }
    }
    fclose(fp);
}

// -v : 입력한 패턴이 포함되지 않은 문자열을 출력한다.
void option_v(char* filename, char* findstr, int find_length){
    FILE *fp;
    char* line, buffer[256];
    int line_length;

    fp = fopen(filename, "r");
    while(1){
        line = fgets(buffer, 256, fp);
        line_length = strlen(line);
        if(line == NULL){
            break;
        }
        if(KMP(findstr, line, line_length, find_length) == NULL){    //KMP의 리턴값이 NULL이면, 문자열 출력
            printf("%s", line);
        }
    }
    fclose(fp);
}

// -n : 일치한 문자열이 포함된 라인 번호를 출력한다.
void option_n(char* filename, char* findstr, int find_length){
    FILE *fp;
    int linecnt = 0, line_length;
    char* line, buffer[256];

    fp = fopen(filename, "r");
    while(1){
        line = fgets(buffer, 256, fp);    //fgets와 함께 linecnt++;
        if(line == NULL){
            break;
        }
        line_length = strlen(line);
        linecnt++;
        if(KMP(findstr, line, line_length, find_length) != NULL){//KMP의 리턴값이 1이면, linecnt출력;
            printf("%d ", linecnt);
        }
    }
    fclose(fp);
}

// -w : 입력한 문자열이 독립된 단어로 존재하는 경우만 출력한다.
void option_w(char* filename, char* findstr, int find_length){
    FILE *fp;
    char* line, buffer[256];
    int line_length;
    
    fp = fopen(filename, "r");
    while(1){
        line = fgets(buffer, 256, fp);
        if(line == NULL)
            break;
        line_length = strlen(line);

        if(KMP(findstr, line, line_length, find_length) != NULL){
            //시작 index + strlen(findstr) + 1의 값이 ' ' || '\0' 일때만 출력
        }
    }
    fclose(fp);
}
