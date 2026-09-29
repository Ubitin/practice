// 后缀表达式求值（数字以 '.' 结尾，'@' 输出结果）—— 修正版 v2
// 针对用户第二版（getchar + do/while）修掉两处：
//   ① 【数字全变 0】ll temp = 0; 写在 do 循环【内部】⇒ 每轮归零
//      ⇒ 多位数只保留最后一位、'.' 处压入 0（实测：9.@→0、12.@→0、5.3.-@→0）
//      ⇒ 修法：temp 声明在循环【外面】
//   ② 【输入结束即崩/空转】循环条件是 while(c != '@')，没判 EOF（getchar 返回 -1）
//      ⇒ EOF 不等于 '@' ⇒ 继续循环 ⇒ 掉进 "else if(c != '@')" 分支
//         ⚠️ 而该分支里 s.top();s.pop(); 写在 switch【外面】⇒ 每次非法字符都弹两次栈
//         ⇒ 栈弹空后 s.top() on empty ⇒ 0xC0000005；若栈非空则死循环弹栈压 0
//      ⇒ 修法：① 循环条件加 c != EOF；② pop 只在真正的运算符分支里做
//      ⇒ 另：除法前判 x==0（否则整数除零 0xC0000094，实测评测机 RE）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    stack<ll> s;
    ll temp = 0;                       // ★ 关键 1：在循环外面，数字才能跨字符累积
    bool haveNum = false;              // 标记 temp 里是否有未入栈的数字
    int c;

    while ((c = getchar()) != EOF && c != '@') {   // ★ 关键 2：同时判 EOF
        if (c >= '0' && c <= '9') {
            temp = temp * 10 + (c - '0');
            haveNum = true;
        } else if (c == '.') {
            s.push(temp);
            temp = 0;
            haveNum = false;
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (s.size() < 2) continue;             // ★ 防御：操作数不够就跳过，别崩
            ll x = s.top(); s.pop();                // 先弹出的是【右】操作数
            ll y = s.top(); s.pop();                // 后弹出的是【左】操作数
            ll res = 0;
            if (c == '+') res = y + x;
            else if (c == '-') res = y - x;         // ★ y - x，不是 x - y
            else if (c == '*') res = y * x;
            else res = (x == 0) ? 0 : y / x;        // ★ 防御：除零
            s.push(res);
        }
        // 其它字符（空格、换行）直接忽略
    }

    if (!s.empty()) cout << s.top() << "\n";
    else if (haveNum) cout << temp << "\n";
    return 0;
}
