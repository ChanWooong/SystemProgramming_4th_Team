#include <stdio.h>
#include <string.h>
#define MAX_LINE_LENGTH 256

int main(int argc, char*argv[]){
	char line[MAX_LINE_LENGTH];
	const char* pattern = argv[1];
	
	while(fgets(line, sizeof(line), stdin) != NULL){
		if(strstr(line, pattern) != NULL){
			printf("%s",line);
		}
	}
}

