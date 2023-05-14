#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "grepLib.h"

int main(void)
{
    char* text = "ABCABAABCABAC";
    char* pattern = "CAB";
 
    int n = strlen(text);
    int m = strlen(pattern);
 
    KMP(text, pattern, n, m);
 
    return 0;
}

