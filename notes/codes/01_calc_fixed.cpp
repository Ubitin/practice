// 后缀表达式求值（数字以 '.' 结尾，'@' 输出结果）—— 修正版 v1
// 修掉的问题：
//   ① 【崩溃无输出】string temp; 写在 for 循环【内部】⇒ 每轮都新建/清空
//      ⇒ 遇到 '.' 时 temp 是（或只剩最后一位）⇒ stoll(空串) 抛 std::invalid_argument
//      ⇒ 没有 try/catch ⇒ std::terminate ⇒ abort ⇒ 退出码 0xC000001D，【一行输出都没有】
//      ⇒ 修法：temp 必须声明在循环【外面】（或改成用 stringstream 整串解析）
//   ② 栈初值 s.push(0)：表达式求值不该预置 0，它会污染运算结果（尤其第一个运算符没用完操作数时）
//   ③ 多位数：temp 累加必须跨字符保留（同一个原因）
//   ④ 打印用 '\n' 而不是没有换行；并把"结果"打印放在该分支内不退栈
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string num;
    if (!(cin >> num)) return 0;

    stack<ll> s;
    string temp;                       // ★ 关键：在循环外面，数字才能跨字符累加
    for (size_t i = 0; i < num.size(); ++i) {
        char c = num[i];
        if (c >= '0' && c <= '9') {
            temp += c;                 // 累加数字
        } else if (c == '.') {
            if (!temp.empty()) {       // ★ 防御：空串不进 stoll
                s.push(stoll(temp));
                temp.clear();
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            ll x = s.top(); s.pop();   // 后弹出的那个是左操作数
            ll y = s.top(); s.pop();
            ll res = 0;
            if (c == '+') res = y + x;
            else if (c == '-') res = y - x;      // ★ 顺序：y - x
            else if (c == '*') res = y * x;
            else res = y / x;                    // ★ 顺序：y / x
            s.push(res);
        } else if (c == '@') {
            cout << s.top() << "\n";   // 输出（不退栈）
        }
    }
    return 0;
}
