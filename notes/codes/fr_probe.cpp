// 探针：看看 s1="1/2" 经过他写的那两行之后，变量里到底是什么
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    string s1 = "1/2";
    ll pos1 = s1.find('/');

    // ① 他的原写法：substr 的返回值没赋给任何东西（只产生一次临时对象，随即销毁）
    string mom1 = s1, son1 = s1;
    son1.substr(0, pos1); mom1.substr(pos1 + 1);
    printf("① 原写法:  son1=\"%s\"  mom1=\"%s\"   → stoi(son1)=%d stoi(mom1)=%d\n",
           son1.c_str(), mom1.c_str(), stoi(son1), stoi(mom1));

    // ② 正确写法：把返回值赋回去
    string mom2 = s1, son2 = s1;
    son2 = son2.substr(0, pos1); mom2 = mom2.substr(pos1 + 1);
    printf("② 正确写法: son2=\"%s\"  mom2=\"%s\"   → stoi(son2)=%d stoi(mom2)=%d\n",
           son2.c_str(), mom2.c_str(), stoi(son2), stoi(mom2));

    // ③ 顺便看清 stoi 的"容忍度"：它会停在第一个非法字符
    printf("③ stoi(\"1/2\") = %d   （遇 '/' 就停，所以会把 1/2 读成 1！）\n", stoi(string("1/2")));
    return 0;
}
