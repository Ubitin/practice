#include <bits/stdc++.h>
using namespace std;

void show(const char* tag, const char* s) { printf("%-18s [%s]\n", tag, s); }

int main() {
    // ===== 1. 声明与初始化（六种方式） =====
    char a[10];                    // ① 未初始化：内容是垃圾值
    char b[10] = {'h', 'i', '\0'}; // ② 逐个字符初始化，手动补 '\0'
    char c[10] = "hi";             // ③ 字符串常量初始化：编译器自动补 '\0'
    char d[] = "hello";            // ④ 不写大小：自动按长度+1 分配（6 字节）
    char e[10] = {0};              // ⑤ 全 0：相当于空字符串
    char f[10];                    // ⑥ 运行时填充（见下文 scanf/fgets）

    printf("sizeof(d) = %zu (6: h e l l o \\0)\n", sizeof(d));
    printf("strlen(d) = %zu (5: 数到 \\0 为止，不含 \\0)\n", strlen(d));

    // ===== 2. 下标遍历 / 指针遍历 / 修改 =====
    char s[] = "hello";
    for (int i = 0; s[i] != '\0'; i++) putchar(s[i]);
    putchar('\n');
    s[0] = 'H';                    // 字符数组可改（可写内存）
    show("修改后", s);             // Hello

    // ===== 3. char 数组 vs char* 字符串常量 =====
    char arr[] = "abc";            // 栈上的可写数组（副本）
    const char* p = "abc";         // 指向字符串常量区（只读！）
    printf("arr 可改 [OK] | p 指向只读区，写 *p 是未定义行为 [X]\n");
    printf("sizeof(arr)=%zu, sizeof(p)=%zu (p 是指针，固定 8 字节)\n", sizeof(arr), sizeof(p));

    // ===== 4. 字符串函数族（对应 C++ string 的方法） =====
    char buf[64] = "hello";
    strcpy(buf, "world");          // 复制 → world
    show("strcpy 后", buf);
    strcat(buf, "!!");             // 拼接 → world!!
    show("strcat 后", buf);
    printf("strcmp(\"abc\",\"abd\") = %d  (<0 表示前者字典序更小)\n", strcmp("abc", "abd"));
    printf("strcmp(\"abc\",\"abc\") = %d  (=0 表示相等)\n", strcmp("abc", "abc"));
    printf("strncpy 更安全：只拷前 n 个字符\n");

    // ===== 5. 输入：scanf %s 遇空白停；fgets 读整行 =====
    char name[32];
    printf("请输两个单词（用空格分开）: ");
    scanf("%s", name);             // 只读到第一个词（数组名不用 &）
    printf("scanf %%s 读到: [%s]\n", name);
    getchar();                     // 吃掉残留的换行符
    printf("请输入一行: ");
    fgets(name, 32, stdin);        // 读整行（含空格），末尾带 '\n'
    printf("fgets 读到: [%s]\n", name);

    // ===== 6. 二维字符数组 = 字符串数组 =====
    char words[3][16] = {"apple", "banana", "cherry"};
    for (int i = 0; i < 3; i++) printf("words[%d] = %s (strlen=%zu)\n", i, words[i], strlen(words[i]));

    return 0;
}
