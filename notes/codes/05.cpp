#include <bits/stdc++.h>
using namespace std;

// 反转字符串并去掉前导 0；若结果全 0 则返回 "0"
string rev(string s) {
    reverse(s.begin(), s.end());
    int i = 0;
    while (i < (int)s.size() && s[i] == '0') i++;
    if (i == (int)s.size()) return "0";   // 全是 0 → "0"
    return s.substr(i);
}

int main() {
    string s;
    cin >> s;

    if (s.find('%') != string::npos) {            // 百分数
        cout << rev(s.substr(0, s.find('%'))) << "%\n";
    } else if (s.find('/') != string::npos) {     // 分数
        size_t p = s.find('/');
        cout << rev(s.substr(0, p)) << "/" << rev(s.substr(p + 1)) << "\n";
    } else if (s.find('.') != string::npos) {     // 小数
        size_t p = s.find('.');
        string a = s.substr(0, p);                // 整数部分
        string b = s.substr(p + 1);               // 小数部分
        // 关键规则：小数部分"看成一个整数"——先剥掉前导 0，再反转
        int j = 0;
        while (j < (int)b.size() && b[j] == '0') j++;
        b = (j == (int)b.size()) ? "0" : b.substr(j);
        cout << rev(a) << "." << rev(b) << "\n";
    } else {                                      // 整数
        cout << rev(s) << "\n";
    }
    return 0;
}
