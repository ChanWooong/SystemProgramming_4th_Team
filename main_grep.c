#include <stdio.h>
#include <stdlib.h>
#include "grepLib.h"

int main(int argc, char* argv[]){
	printf("%s", argv[2]);
	printf("h");
	char b = getchar();
	printf("%c",b);
	pipeLine(argv[1], argv[3], argv[4]);
}
