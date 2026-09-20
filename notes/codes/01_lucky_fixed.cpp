#include <bits/stdc++.h>
using namespace std;

// 质数判断：x<2 不是质数；只需试除到 sqrt(x)
bool isPrime(int x) {
    if (x < 2) return false;
    for (int j = 2; j * j <= x; ++j)
        if (x % j == 0) return false;
    return true;
}

int main() {
    string s;
    cin >> s;

    int cnt[26] = {0};                // ① 计数数组：统计每个字母出现次数
    for (char c : s) cnt[c - 'a']++;

    int maxn = 0, minn = 1e9;
    for (int i = 0; i < 26; i++) {    // ② 只统计"出现过的字母"
        if (cnt[i] > 0) {
            maxn = max(maxn, cnt[i]);
            minn = min(minn, cnt[i]);
        }
    }

    int d = maxn - minn;              // ③ 差值
    if (isPrime(d))                   // ④ 质数判断
        cout << "Lucky Word\n" << d;
    else
        cout << "No Answer\n" << 0;
    return 0;
}
