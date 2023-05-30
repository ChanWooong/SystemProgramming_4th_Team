#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"
#include <stdbool.h>
#include <signal.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#define BUFFERSIZ 3
#define MAX 3000

volatile sig_atomic_t flag = 0;

void signal_handler(int signal) {
	if(signal == SIGINT)
		flag = 1;
}

#ifdef _WIN32
void clear() {
	system("cls");
}
#else
void clear() {
	printf("\033[H\033[J");
}
#endif


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



int main(int argc, char* argv[])
{
    FILE* fd;
    char cur_line[MAX] = {0};
    char find[MAX];
    char pre_line[BUFFERSIZ][MAX] = {0};
    char next_line[MAX] = {0};
    int index = 0;
    bool found = false;
    bool skip = false;
    bool print_cur_line = false;
    bool print_next_line = false;
    bool input = false;

    if (argc != 3)
    {
        printf("Check the number of parameters.\n");
        exit(1);
    }

    fd = fopen(argv[2], "r");

    if (fd == NULL)
    {
        printf("Cannot open file\n");
        exit(1);
    }

    setup();

   
    while (fgets(cur_line, MAX, fd) != NULL)
    {	
	clear();

	strcpy(find, argv[1]);
	printf("Searching for [ %s ] \n\n", argv[1]);

        if (strstr(cur_line, argv[1]) != NULL) {
	
            found = true;
	    skip = true;

            int n = strlen(cur_line);
            int m = strlen(find);
	          
	    int pre_index = (index-1+BUFFERSIZ)%BUFFERSIZ;
	    if(strlen(pre_line[pre_index]) > 0) {
		    printf("%s", pre_line[pre_index]);
	    }
	

	    printf("\n");

	    KMP(cur_line, find, n, m);
	    printf("\n");
	    
	    strncpy(pre_line[index], cur_line, MAX);       
	    index = (index + 1) % BUFFERSIZ;

	    if(fgets(next_line, MAX, fd)!= NULL) {
		   printf("%s", next_line);
		   print_next_line = true;
	    }
	    else {
		    printf("End of file ! \n");
	    }

            input = true;
	}
	else {
		strncpy(pre_line[index], cur_line, MAX);
		index = (index + 1) % BUFFERSIZ;
		skip = true;
	}

	if(flag)
		break;

        if(input) {
	    while(1) {
		printf("\n\n\nn/N : next\nq/Q : quit\n\n\n");
                int ch = getch();
                if(tolower(ch) == 'n')
                           break;
                else if(tolower(ch) == 'q') {
                           flag = 1;
                           break;
                        }
                else if(!isspace(ch)) {
                           printf("Only n/N or q/Q !\n");
                        }
	}
	input = false;
	}
    }

    if (!found) {
        printf("Cannot find pattern\n");
    }

    fclose(fd);

    return 0;
   
}


