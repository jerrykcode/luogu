#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
using namespace std;

#define maxlen 2000
char buf[maxlen];
uint8_t num[2][maxlen];
size_t numlen[2];
#define a num[0]
#define b num[1]
#define alen numlen[0]
#define blen numlen[1]

uint32_t c[maxlen << 1];

/*
void get(char * buf) {
    int ch, idx = 0;
    while ((ch = getchar()) != '\n' && ch != EOF) buf[idx++] = ch;
    buf[idx] = '\0';
}
*/

int main () {
    for (int i = 0; i < 2; i++) {
//        fgets(buf, maxlen, stdin);
//        int len = strlen(buf) - 1;
//        get(buf);
        cin >> buf;
        int len = strlen(buf);
        for (int j = 0; j < len; j++)
            num[i][j] = buf[len - j - 1] - '0';
        numlen[i] = len;
    }

    for (int i = 0; i < alen; i++)
        for (int j = 0; j < blen; j++)
            c[i + j] += (uint32_t)a[i] * (uint32_t)b[j];


    for (int i = 0; i < alen + blen; i++) {
        c[i + 1] += c[i] / 10;
        c[i] %= 10;
    }

    int len = alen + blen;
    while (len && c[len] == 0) len--;
    for ( ; len >= 0; len--) {
        //fprintf(stdout, "%u", c[len]);
        //printf("%u", c[len]);
        putchar(c[len] + '0');
    }

    return 0;
}
