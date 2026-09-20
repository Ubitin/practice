#include <bits/stdc++.h>
using namespace std;

void show(char s[]) { printf("%zu ", sizeof(s)); }   // A1
void show2(char* p) { printf("%zu\n", strlen(p)); }  // A2

int main() {
    // A1: 传参退化（Q2 变式）
    char s[] = "abcd";
    show(s);
    printf("%zu\n", sizeof(s));                      // 期望: 8 5

    // A2: strlen 不受退化影响
    char t[] = "abcd";
    show2(t);                                        // 期望: 4

    // B2: char[] vs char* 的 sizeof（Q4 变式）
    char a[] = "hi";
    char* p = "hi";
    printf("B2: %zu %zu\n", sizeof(a), sizeof(p));   // 期望: 3 8

    // C2: 缺 '\0' 但能编译（Q5 变式）——不打印 strlen，避免 UB；只演示 sizeof
    char c[3] = {'h', 'i'};                          // 编译通过，但 c[2] 是垃圾值
    printf("C2: sizeof(c)=%zu, 但 c[2] 没有 '\\0'，strlen(c) 是未定义行为\n", sizeof(c));
    // 补充: char d[2] = "hi"; 会直接编译错误（initializer-string too long），编译器帮忙拦住

    // D1: scanf 三个词（Q7 变式）
    char x[16], y[16], z[16];
    scanf("%s%s%s", x, y, z);                        // 输入: I love you
    printf("D1: %s|%s|%s\n", x, y, z);               // 期望: I|love|you

    // D2: 数字 + 字符串混读
    int n; char w[16];
    scanf("%d%s", &n, w);                            // 输入: 42 hello
    printf("D2: %d %s\n", n, w);                     // 期望: 42 hello

    // E1: 二维 sizeof（Q9 变式）
    char m[3][10] = {"a", "bb", "ccc"};
    printf("E1: %zu %zu %zu %zu\n", sizeof(m), sizeof(m[0]), strlen(m[1]), strlen(m[2]));
    // 期望: 30 10 2 3

    return 0;
}
