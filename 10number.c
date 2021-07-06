/*

〇の中に0～9の数字を1つずつ、また●の中にも0～9の数字を1つずつ入れて式を完成させよ。
　　　　〇〇〇〇〇〇
　　　×　　〇〇〇〇
――――――――――
●●●●●●●●●●

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int multiple(char* x, int y, int z, char* w) {
    int i, j;
    int a = 0;
    int b = 0;
    char *p = w;
    int res;

    if (x[0] == '0' || x[6] == '0') {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        w[i] = 0;
    }
    
    for (i = 0; i < y; i++) {
        a *= 10;
        a += x[i] - '0';
    }
    for (i = 0; i < z; i++) {
        b *= 10;
        b += x[y+i] - '0';
    }
    for (i = 9; i >= 6; i--) {
        res = a*(b % 10);
        j = i;
        while (res != 0) {
            w[j] += res % 10;
            res /= 10;
            j--;
        }
        b /= 10;
    }

    for (i = 9; i >= 1; i--) {
        if (w[i] >= 10) {
            w[i-1] += w[i] / 10;
            w[i] = w[i] % 10;
        }
    }

    if (w[0] == 0) {
        return 0;
    }
    return 1;
}

int isOneElement(char *c) {
    int i;
    char n[11];
    char num[2];
    for (i = 0; i < 11; i++) {
        if (i == 10) {
            n[i] = 0;
        } else {
            n[i] = c[i] + '0';
        }
    }
    for (i = 0; i < 10; i++) {
        sprintf(num, "%d", i);
        if (strstr(n, num) == NULL) {
            return 0;
        }
    }
    return 1;
}

int main() {
    char a[12];
    char c[10];
    FILE *fp;
    int i;
    int count = 0;
    fp = fopen("permutation.txt", "r");
    if (fp == NULL) {
        printf("FILE read fail!");
        return -1;
    }
    for (i = 0; i < 10; i++) {
        c[i] = i;
    }

    while (fgets(a, 12, fp) != NULL) {
        if (multiple(a, 6, 4, c)) {
            if (isOneElement(c)) {
                count++;
                printf("%d -> ", count);
                for (i = 0; i < 6; i++) {
                    printf("%c", a[i]);
                }
                printf(" * ");
                for (i = 6; i < 10; i++) {
                    printf("%c", a[i]);
                }
                printf(" = ");
                for (i = 0; i < 10; i++) {
                    printf("%d", c[i]);
                }
                printf("\n");
            }
        }
    }
    return 0;
}