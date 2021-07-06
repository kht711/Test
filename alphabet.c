/*

　　　　　　　　　　　　　　　ＩＮＴＯ
　　　　　　　　　　　　　　　ＯＮＴＯ
　　　　　　　　　　　　　　ＣＡＮＯＮ
　　　　　　　　　　　　　ＩＮＴＡＣＴ
　　　　　　　　　　　　ＡＭＭＯＮＩＡ
　　　　　　　　　　　ＯＭＩＳＳＩＯＮ
　　　　　　　　　　ＤＩＡＣＲＩＴＩＣ
　　　　　　　　　ＳＴＡＴＩＳＴＩＣＳ
　　　　　　　　ＡＳＳＯＣＩＡＴＩＯＮ
　　　　　　　ＡＮＴＩＭＡＣＡＳＳＡＲ
　　　　　　ＣＯＮＴＯＲＴＩＯＮＩＳＴ
　　ＮＯＮＤＩＳＣＲＩＭＩＮＡＴＩＯＮ
＋　ＣＯＮＴＲＡＤＩＳＴＩＮＣＴＩＯＮ
--------------------------------------
　　ＭＩＳＡＤＭＩＮＩＳＴＲＡＴＩＯＮ

アルファベットにそれぞれ異なる１ケタの数字（０～９）をあてはめて、式を完成させよ。ただし同じアルファベットには同じ数字が入る。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void allSum(int* res, int** x) {
    int** p = x;
    int* q = res;
    int count = 0;
    while (*p != NULL) {
        count++;
        p++;
    }
    p = x;
    while (*p != NULL) {
        q[17 - count] += **p;
        p++;
        q++;
    }
}

int main() {
    int a, c, d, i, m, n, o, r, s, t;
    char var[10] = {'a', 'c', 'd', 'i', 'm', 'n', 'o', 'r', 's', 't'};
    FILE* fp;
    int index;
    int* p[10] = { &a, &c, &d, &i, &m, &n, &o, &r, &s, &t };
    char q[12];
    int* into[5] = { &i, &n, &t, &o, NULL };
    int* onto[5] = { &o, &n, &t, &o, NULL };
    int* canon[6] = { &c, &a, &n, &o, &n, NULL };
    int* intact[7] = { &i, &n, &t, &a, &c, &t, NULL };
    int* ammonia[8] = { &a, &m, &m, &o, &n, &i, &a, NULL };
    int* omission[9] = { &o, &m, &i, &s, &s, &i, &o, &n, NULL };
    int* diacritic[10] = { &d, &i, &a, &c, &r, &i, &t, &i, &c, NULL };
    int* statistics[11] = { &s, &t, &a, &t, &i, &s, &t, &i, &c, &s, NULL };
    int* association[12] = { &a, &s, &s, &o, &c, &i, &a, &t, &i, &o, &n, NULL };
    int* antimacassar[13] = { &a, &n, &t, &i, &m, &a, &c, &a, &s, &s, &a, &r, NULL };
    int* contortionist[14] = { &c, &o, &n, &t, &o, &r, &t, &i, &o, &n, &i, &s, &t, NULL };
    int* nondiscrimination[18] = { &n, &o, &n, &d, &i, &s, &c, &r, &i, &m, &i, &n, &a, &t, &i, &o, &n, NULL };
    int* contradistinction[18] = { &c, &o, &n, &t, &r, &a, &d, &i, &s, &t, &i, &n, &c, &t, &i, &o, &n, NULL };
    int* misadministration[17] = { &m, &i, &s, &a, &d, &m, &i, &n, &i, &s, &t, &r, &a, &t, &i, &o, &n };
    int res[17];
    int flag;

    fp = fopen("permutation.txt", "r");
    if (fp == NULL) {
        printf("FILE read fail!\n");
        return -1;
    }

    while (fgets(q, 12, fp) != NULL) {
        flag = 1;
        for (index = 0; index < 17; index++) {
            res[index] = 0;
        }
        for (index = 0; index < 10; index++) {
            *p[index] = q[index] - '0';
        }
        allSum(res, into);
        allSum(res, onto);
        allSum(res, canon);
        allSum(res, intact);
        allSum(res, ammonia);
        allSum(res, omission);
        allSum(res, diacritic);
        allSum(res, statistics);
        allSum(res, association);
        allSum(res, antimacassar);
        allSum(res, contortionist);
        allSum(res, nondiscrimination);
        allSum(res, contradistinction);

        index = 16;
        while (index != 0) {
            if (res[index] >= 10) {
                res[index-1] += (res[index] / 10);
                res[index] = res[index] % 10;
            }
            index--;
        }

        for (index = 0; index < 17; index++) {
            if (res[index] != *misadministration[index]) {
                flag = 0;
                break;
            }
        }
        if (flag) {
            for (index = 0; index < 10; index++) {
                printf("%c -> ", var[index]);
                printf("%d\n", *p[index]);
            }
            printf("\n");
        }
    }
    return 0;
}