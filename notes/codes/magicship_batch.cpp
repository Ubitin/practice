// CF1117C 对拍：二分+前缀和 正解 vs 逐天扩散暴力
// 批量输入：一次生成 K 个用例，两个程序各跑一次逐行比对
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
static const string DIR = "D:\\lenovo\\chat_with_deepseek_harness\\test_\\";

int main(int argc, char** argv) {
    int K = (argc > 1) ? atoi(argv[1]) : 2000;
    unsigned seed = (argc > 2) ? (unsigned)atoi(argv[2]) : 1u;
    mt19937 rng(seed);

    stringstream in;
    for (int t = 0; t < K; ++t) {
        int style = t % 6;
        int R = (style == 0) ? 3 : (style == 1) ? 6 : (style == 2) ? 10 : 15;   // 目标范围
        int n = 1 + rng() % 6;                                                 // 风序列长度
        ll x1 = rng() % 5, y1 = rng() % 5;
        ll x2 = x1 + (ll)(rng() % (2 * R + 1)) - R;
        ll y2 = y1 + (ll)(rng() % (2 * R + 1)) - R;
        string w;
        const char* ch = "UDLR";
        if (style == 3) {                       // 全是同一个方向
            char c = ch[rng() % 4];
            w = string(n, c);
        } else if (style == 4) {                // 只用 U/D（只有一维有风）
            for (int i = 0; i < n; ++i) w += (rng() % 2 ? 'U' : 'D');
        } else if (style == 5) {                // 风相互抵消的周期
            for (int i = 0; i < n; ++i) w += ch[(i / 2) % 2 * 2 + (i % 2)];
        } else {
            for (int i = 0; i < n; ++i) w += ch[rng() % 4];
        }
        in << x1 << " " << y1 << " " << x2 << " " << y2 << " " << n << "\n" << w << "\n";
    }
    { ofstream f(DIR + "ms_batch_in.txt", ios::binary); f << in.str(); }

    auto run = [&](const string& exe) {
        system((DIR + exe + " < " + DIR + "ms_batch_in.txt > " + DIR + "ms_" + exe + ".out").c_str());
        ifstream f(DIR + "ms_" + exe + ".out");
        vector<string> v; string s;
        while (getline(f, s)) { while (!s.empty() && (s.back() == '\r' || s.back() == ' ')) s.pop_back(); if (!s.empty()) v.push_back(s); }
        return v;
    };
    auto B = run("magicship_brute.exe");
    auto F = run("magicship_fixed.exe");
    printf("用例数=%d  暴力输出行数=%d  正解输出行数=%d\n", K, (int)B.size(), (int)F.size());
    int bad = 0, firstBad = -1;
    for (int i = 0; i < K; ++i) {
        string b = i < (int)B.size() ? B[i] : "?";
        string f = i < (int)F.size() ? F[i] : "?";
        if (b != f) { ++bad; if (firstBad < 0) firstBad = i; }
    }
    printf("不一致 %d / %d %s\n", bad, K, bad ? "✘" : "✅");
    if (firstBad >= 0) printf("  首个不一致：第 %d 个用例 暴力=%s 正解=%s\n", firstBad + 1,
                              B[firstBad].c_str(), F[firstBad].c_str());
    return bad ? 7 : 0;
}
