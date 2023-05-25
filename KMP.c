#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int result[BUFSIZ] = {0};
int* KMP(const char* text, const char* pattern, int m, int n)
{
	int arr[20];
	int cnt = 0;

    if (*pattern == '\0' || n == 0) {
        printf("The pattern occurs with shift 0");
    }
 

    if (*text == '\0' || n > m) {
        printf("Pattern not found");
    }
 

    int next[n + 1];
 
    for (int i = 0; i < n + 1; i++) {
        next[i] = 0;
    }
 
    for (int i = 1; i < n; i++)
    {
        int j = next[i];
 
        while (j > 0 && pattern[j] != pattern[i]) {
            j = next[j];
        }
 
        if (j > 0 || pattern[j] == pattern[i]) {
            next[i + 1] = j + 1;
        }
    }
 
    for (int i = 0, j = 0; i < m; i++)
    {
        if (*(text + i) == *(pattern + j))
        {
            if (++j == n) {
                arr[cnt++] = i-j+1;
            }
        }
        else if (j > 0)
        {
            j = next[j];
            i--;   
        }
    }
    int check = 0;
    cnt = 0;

    for(int k = 0; k < strlen(text); k++){
	if(k == arr[cnt]){
		result[check] = k;
	}
	if(k == arr[cnt]+n){
		check++;
		cnt++;
		k--;
		continue;
	}
    }
    if(check == 0){
	printf("There has no String in FIle\n");
    }	
    printf("\n");
    return result;

}
