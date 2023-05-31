#include <curses.h>

int* KMP(const char* text, const char* pattern, int m, int n);
int* result;
WINDOW *win;
WINDOW *win2;
WINDOW *title;
WINDOW *content;

void option_none(char* filename, char* findstr, int find_length);

void option_all(char* findstr, int find_length);

// -r : 서브 디렉터리의 파일까지 모두 출력한다.
void option_r(char *findstr, int find_length);

// -c : 문자열이 포함된 라인 개수를 표시한다.
void option_c(char* filename, char* findstr, int find_length);

// -i : 문자열의 대소문자를 구분하지 않는다.
void option_i(char* filename, char* findstr, int find_length);

// -h : 파일 이름을 출력하지 않는다.
void option_h(char* filename, char* findstr, int find_length);

// -v : 입력한 패턴이 포함되지 않은 문자열을 출력한다.
void option_v(char* filename, char* findstr, int find_length);

// -n : 일치한 문자열이 포함된 라인 번호를 출력한다.
void option_n(char* filename, char* findstr, int find_length);

// -w : 입력한 문자열이 독립된 단어로 존재하는 경우만 출력한다.
void option_w(char* filename, char* findstr, int find_length);

void option_l(char* findstr, int find_length);

char* data_buffer[100];

char** getdata(char* filename);

int getbuffersize(char** buffer);

int getarraysize(int* arr);

void printline(char* line, int* start_index, int find_length, int linenum);

void print_fiveline(int i, char** buffer, int* data_KMP, int find_len, int linenum);

