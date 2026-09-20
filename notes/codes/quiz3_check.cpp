#include <bits/stdc++.h>
using namespace std;
int main() {
    // E1 答案验证
    char cs[] = "abc";
    string ss = "abc";
    printf("E1: %zu %zu %zu %d\n", sizeof(cs), strlen(cs), ss.size(), (int)ss[3]);
    // 期望: 4 3 3 0

    // E4 答案验证
    string a = "abc", b = "abd";
    cout << "E4: " << (a < b) << " " << (a == "abc") << "\n";
    // 期望: 1 1

    // E5 答案验证
    char buf[20];
    scanf("%s", buf);              // 输入 hello world → 只读 hello
    printf("E5: %zu\n", strlen(buf));
    // 期望: 5

    // E6 概念验证：c_str() 指针在修改后可能失效
    string s = "hello";
    const char* p = s.c_str();
    s[0] = 'H';
    printf("E6 说明: 修改后 p 指向的内容可能变化/失效 (此处实际输出: %s)\n", p);
    return 0;
}
