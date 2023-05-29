#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int *result;

int* KMP(const char* text, const char* pattern, int m, int n)
{
    result = NULL;
	int arr[20] = {0};
	int cnt = 0;

    if (*pattern == '\0' || n == 0) {
        printf("The pattern occurs with shift 0");
    }
 

    if (*text == '\0' || n > m) {
        return NULL;
    }
 

    int next[n + 1];
 
    for (int i = 0; i < n + 1; i++) {
        next[i] = 0;
    }
    
    for (int i = 1; i < n; i++)//실패함수
    {
        int j = next[i];
 
        while (j > 0 && pattern[j] != pattern[i]) {
            j = next[j];
        }
 
        if (pattern[j] == pattern[i]) {
            next[i + 1] = j + 1;
        }
    }
 
    for (int i = 0, j = 0; i < m; i++)
    {
        if (*(text + i) == *(pattern + j))
        {
            if (++j == n) {
                arr[cnt] = i-j+1;
                if(result == NULL){
                    result = (int*)malloc(sizeof(int));
                }
                else{
                    result = realloc(result, sizeof(int) * (cnt+1));
                }
                result[cnt++] = i-j+1;        
            }
        }
        else if (j > 0)
        {
            j = next[j];
            i--;   
        }
    }

    if(cnt == 0) return NULL;
    
    else return result;
}