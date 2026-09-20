#include <bits/stdc++.h>
using namespace std;

void f(char s[]) { printf("%zu", sizeof(s)); }   // Q2 用

int main() {
    // Q1: sizeof vs strlen
    char s[] = "hello";
    printf("Q1: %zu %zu\n", sizeof(s), strlen(s));          // 期望 6 5

    // Q2: 数组传参退化
    printf("Q2: %zu ", sizeof(s)); f(s); printf("\n");      // 期望 6 8

    // Q3: strcmp
    printf("Q3: %d %d %d\n", strcmp("abc","abc"), strcmp("abc","abd"), strcmp("abd","abc"));
    // 期望 0 -1 1（gcc 返回差值；标准只保证 <0/=0/>0）

    // Q7: scanf 两个 %s
    char a[32], b[32];
    scanf("%s%s", a, b);
    printf("Q7: [%s]|[%s]\n", a, b);                        // 输入 hello world → [hello]|[world]

    // Q8: fgets 保留换行
    char line[16];
    getchar();                        // 吃掉 Q7 残留换行
    fgets(line, 16, stdin);
    printf("Q8: [%s]\n", line);       // 输入 abc 回车 → [abc\n]

    // Q9: 二维字符数组
    char w[2][6] = {"hi", "hello"};
    printf("Q9: %zu %zu %zu\n", sizeof(w), sizeof(w[0]), strlen(w[1]));  // 期望 12 6 5

    // Q12: 修改后遍历
    char t[] = "hello";
    t[1] = 'a';
    printf("Q12: ");
    for (int i = 0; t[i]; i++) putchar(t[i]);
    printf("\n");                                           // 期望 hallo

    return 0;
}
