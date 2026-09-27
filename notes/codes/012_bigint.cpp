// 012 终极版：小于 n 的自然数中 3 或 5 的倍数之和
// 【为什么必须高精度】题目没给数据范围。答案量级 ≈ 0.2667·n²：
//     n=1e9  → 2.7e17   （long long 够）
//     n=1e10 → 2.7e19   （long long 不够！）
//     n=1e18 → 2.7e35   （__int128 够）
//     n=1e19 → 2.7e37   （__int128 也不够 ⇒ 必须高精度）
//   ⇒ 没范围就用高精度，一劳永逸。
//
// 【算法】等差数列 + 容斥，O(1) 每组（大数代价另算）
//   t(k) = (n-1)/k          // < n 的 k 的正倍数个数（"小于 n" 所以要减 1）
//   S(k) = k * (1+2+...+t) = k * t(t+1)/2
//   ans  = S(3) + S(5) - S(15)     // 公倍数 15 的倍数被数了两遍
#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

// ---------- 无符号大整数：十进制每位一字节，低位在前 ----------
struct Big {
    vector<int> d;                                  // d[0] 是最低位
    Big() {}
    Big(ull v) { while (v) { d.push_back(int(v % 10)); v /= 10; } }
    void trim() { while (d.size() > 1 && d.back() == 0) d.pop_back(); }
    bool isZero() const { return d.empty() || (d.size() == 1 && d[0] == 0); }
};

int cmp(const Big& a, const Big& b) {               // -1 / 0 / 1
    vector<int> x = a.d, y = b.d;
    while (x.size() > 1 && x.back() == 0) x.pop_back();
    while (y.size() > 1 && y.back() == 0) y.pop_back();
    if (x.size() != y.size()) return x.size() < y.size() ? -1 : 1;
    for (int i = (int)x.size() - 1; i >= 0; --i) if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

Big add(const Big& a, const Big& b) {
    Big r; int carry = 0;
    size_t n = max(a.d.size(), b.d.size());
    for (size_t i = 0; i < n || carry; ++i) {
        int s = carry;
        if (i < a.d.size()) s += a.d[i];
        if (i < b.d.size()) s += b.d[i];
        r.d.push_back(s % 10); carry = s / 10;
    }
    r.trim(); return r;
}

Big sub(const Big& a, const Big& b) {               // 保证 a >= b
    Big r; int borrow = 0;
    for (size_t i = 0; i < a.d.size(); ++i) {
        int s = a.d[i] - borrow - (i < b.d.size() ? b.d[i] : 0);
        if (s < 0) { s += 10; borrow = 1; } else borrow = 0;
        r.d.push_back(s);
    }
    r.trim(); return r;
}

Big mulSmall(const Big& a, ull k) {                 // 大数 × 小整数
    if (k == 0 || a.isZero()) return Big(0);
    Big r; ull carry = 0;
    for (size_t i = 0; i < a.d.size() || carry; ++i) {
        ull cur = carry;
        if (i < a.d.size()) cur += (ull)a.d[i] * k;
        r.d.push_back(int(cur % 10)); carry = cur / 10;
    }
    r.trim(); return r;
}

Big divSmall(const Big& a, ull k) {                 // 大数 ÷ 小整数（整除，向下取整）
    Big r; r.d.assign(a.d.size(), 0); ull rem = 0;
    for (int i = (int)a.d.size() - 1; i >= 0; --i) {
        ull cur = rem * 10 + (ull)a.d[i];
        r.d[i] = int(cur / k); rem = cur % k;
    }
    r.trim(); return r;
}

// S(k) = k * t(t+1) / 2 ，其中 t = (n-1)/k
Big sumMultiples(const Big& n, ull k) {
    Big nm1 = sub(n, Big(1));                       // n-1
    Big t = divSmall(nm1, k);                       // t = (n-1)/k
    Big t1 = add(t, Big(1));                        // t+1
    Big prod = mulSmall(t, 1);                      // 先复制一份 t
    // 注意顺序：先算 t*t1 再除 2（t 与 t+1 必有一偶数，但除在乘法前会截断，所以先乘）
    Big tt1 = Big(0);
    {   // tt1 = t * (t+1)：用"大数 × 大数"里最简单的情形——把 t1 拆成十进制位逐位乘
        vector<Big> part(10);
        for (int dig = 0; dig < 10; ++dig) part[dig] = mulSmall(t, (ull)dig);
        Big acc(0);
        for (int i = (int)t1.d.size() - 1; i >= 0; --i) {
            acc = mulSmall(acc, 10);
            acc = add(acc, part[t1.d[i]]);
        }
        tt1 = acc;
    }
    Big half = divSmall(tt1, 2);                    // t(t+1)/2
    return mulSmall(half, k);                       // × k
}

void printBig(const Big& a) {
    if (a.d.empty()) { printf("0"); return; }
    for (int i = (int)a.d.size() - 1; i >= 0; --i) printf("%d", a.d[i]);
}

// ---------- 读入大整数 n ----------
Big readBig() {
    string s; cin >> s;
    Big r; r.d.resize(s.size());
    for (size_t i = 0; i < s.size(); ++i) r.d[s.size() - 1 - i] = s[i] - '0';
    r.trim();
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        Big n = readBig();
        Big s3 = sumMultiples(n, 3);
        Big s5 = sumMultiples(n, 5);
        Big s15 = sumMultiples(n, 15);
        Big ans = add(s3, s5);
        ans = sub(ans, s15);
        printBig(ans);
        printf("\n");
    }
    return 0;
}
