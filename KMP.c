#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
// KMP 알고리즘을 구현하는 함수
void KMP(const char* text, const char* pattern, int m, int n)
{
	int arr[20];
	int cnt = 0;
    // 기본 사례 1: 패턴이 NULL이거나 비어 있음
    if (*pattern == '\0' || n == 0) {
        printf("The pattern occurs with shift 0");
    }
 
    // 기본 경우 2: 텍스트가 NULL이거나 텍스트의 길이가 패턴의 길이보다 작음
    if (*text == '\0' || n > m) {
        printf("Pattern not found");
    }
 
    // next[i]는 다음으로 가장 좋은 부분 일치의 인덱스를 저장합니다.
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
            i--;    // `i`는 다음 반복에서 증가할 것이기 때문에
        }
    }
    cnt = 0;
    for(int k = 0; k < m; k++){
	if(k == arr[cnt]){
		printf("[");	
	}
	if(k == arr[cnt]+n){
		printf("]");
		cnt++;
	}
	printf("%c",text[k]);
    }
    printf("\n");
}
