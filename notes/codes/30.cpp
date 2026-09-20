// 字符串统计演示：count/count_if、子串计数、桶计数、前缀频次 + 性能实测
// 配套讲义：讲义/字符串统计详解-count与桶计数.md
// 注意：本机 g++ 按 GBK 解码源文件，字符串字面量里不要放中文（会报 expected ';' before ')'）
#include <bits/stdc++.h>
using namespace std;
using Clock = chrono::steady_clock;

int main() {
    cout << "=== (1) count / count_if ===\n";
    string s = "Banana 2024, banana!";
    cout << "s = \"" << s << "\"\n";
    cout << "count(s,'a')      = " << count(s.begin(), s.end(), 'a') << "\n";
    cout << "count(s,'B')      = " << count(s.begin(), s.end(), 'B') << "\n";
    cout << "count(s,'b')      = " << count(s.begin(), s.end(), 'b') << "\n";
    cout << "count_if digit    = "
         << count_if(s.begin(), s.end(), [](char c) { return isdigit((unsigned char)c); }) << "\n";
    cout << "count_if alpha    = "
         << count_if(s.begin(), s.end(), [](char c) { return isalpha((unsigned char)c); }) << "\n";
    cout << "count_if upper    = "
         << count_if(s.begin(), s.end(), [](char c) { return isupper((unsigned char)c); }) << "\n";
    cout << "count_if !space   = "
         << count_if(s.begin(), s.end(), [](char c) { return !isspace((unsigned char)c); }) << "\n";

    // (2) 手动循环等价 + 返回类型
    int manual = 0;
    for (char ch : s) if (ch == 'a') ++manual;
    auto ret = count(s.begin(), s.end(), 'a');
    cout << "\n=== (2) manual loop & return type ===\n";
    cout << "manual loop       = " << manual << "  (same as count)\n";
    cout << "sizeof(count ret) = " << sizeof(ret) << " bytes, typeid = " << typeid(ret).name() << "\n";

    // (3) 子串计数：不重叠 vs 允许重叠
    cout << "\n=== (3) substring count ===\n";
    auto count_sub = [](const string& t, const string& sub, bool overlap) {
        int c = 0;
        size_t step = overlap ? 1 : sub.size();
        for (size_t p = t.find(sub); p != string::npos; p = t.find(sub, p + step)) ++c;
        return c;
    };
    cout << "\"aaaa\" / \"aa\"    : no-overlap = " << count_sub("aaaa", "aa", false)
         << ", overlap = " << count_sub("aaaa", "aa", true) << "\n";
    cout << "\"abababa\" / \"aba\": no-overlap = " << count_sub("abababa", "aba", false)
         << ", overlap = " << count_sub("abababa", "aba", true) << "\n";
    cout << "\"aaaa\" / \"b\"     : no-overlap = " << count_sub("aaaa", "b", false)
         << "  (find returns npos -> 0, no endless loop)\n";

    // (4) 桶计数：每个字母各几次（忽略大小写）
    cout << "\n=== (4) bucket count (one pass) ===\n";
    int bucket[26] = {0};
    for (char ch : s) if (isalpha((unsigned char)ch)) bucket[tolower(ch) - 'a']++;
    cout << "freq: ";
    for (int i = 0; i < 26; ++i)
        if (bucket[i]) cout << char('a' + i) << "=" << bucket[i] << " ";
    cout << "\n";
    for (int i = 0; i < 26; ++i) {
        int byCount = count_if(s.begin(), s.end(), [i](char c) {
            return isalpha((unsigned char)c) && tolower(c) - 'a' == i;
        });
        if (byCount != bucket[i]) { cout << "MISMATCH\n"; return 1; }
    }
    cout << "cross-check with 26x count_if: identical\n";

    // (5) 前缀频次：区间内某字符出现次数
    cout << "\n=== (5) prefix frequency ===\n";
    string t = "abacabadabacaba";
    int n = t.size();
    vector<array<int, 26>> pre(n + 1);
    pre[0].fill(0);
    for (int i = 0; i < n; ++i) {
        pre[i + 1] = pre[i];
        pre[i + 1][t[i] - 'a']++;
    }
    cout << "t = " << t << "\n";
    int bad = 0, checks = 0;
    for (auto [l, r] : vector<pair<int, int>>{{1, 4}, {3, 9}, {5, 5}, {1, n}, {7, 15}}) {
        for (char ch : string("abcd")) {
            int fast = pre[r][ch - 'a'] - pre[l - 1][ch - 'a'];
            int slow = count(t.begin() + (l - 1), t.begin() + r, ch);
            ++checks;
            if (fast != slow) ++bad;
        }
    }
    cout << "count of 'a' in [1,4] = " << pre[4]['a' - 'a'] - pre[0]['a' - 'a'] << "\n";
    cout << checks << " range queries vs count(): " << (bad ? "MISMATCH" : "all identical") << "\n";

    // (6) 坑：vector<int> 上写 'a' 比较的是 97
    cout << "\n=== (6) type mismatch trap ===\n";
    vector<int> v = {97, 98, 97};
    cout << "count({97,98,97}, 'a') = " << count(v.begin(), v.end(), 'a')
         << "  <- 'a' is treated as 97\n";

    // (7) 性能实测：26 次 count（26 遍） vs 桶计数（1 遍）
    cout << "\n=== (7) performance, n = 1e6 ===\n";
    mt19937 rng(1);
    string big(1000000, 'a');
    for (char& ch : big) ch = char('a' + rng() % 26);
    auto t0 = Clock::now();
    long long sumA = 0;
    for (int c = 0; c < 26; ++c) sumA += count(big.begin(), big.end(), char('a' + c));
    auto t1 = Clock::now();
    long long sumB = 0;
    int cnt2[26] = {0};
    for (char ch : big) cnt2[ch - 'a']++;
    for (int i = 0; i < 26; ++i) sumB += cnt2[i];
    auto t2 = Clock::now();
    auto ms = [](auto a, auto b) { return chrono::duration<double, milli>(b - a).count(); };
    cout << "26 x count   : " << ms(t0, t1) << " ms (sum = " << sumA << ")\n";
    cout << "one-pass bucket: " << ms(t1, t2) << " ms (sum = " << sumB << ")\n";
    cout << "ratio approx : " << ms(t0, t1) / ms(t1, t2) << " x\n";
    return 0;
}
