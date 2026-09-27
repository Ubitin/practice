// 国家补助题对拍：暴力解 vs 你的原码 vs 修正版
// 用法: p7990_stress.exe [rounds] [seed]
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static const string DIR = "D:\\lenovo\\chat_with_deepseek_harness\\test_\\";

static string norm(string s) {
    string r;
    for (char c : s) if (c != '\r') r += c;
    while (!r.empty() && (r.back() == '\n' || r.back() == ' ')) r.pop_back();
    size_t i = 0; while (i < r.size() && r[i] == ' ') ++i;
    return r.substr(i);
}
static string runOne(const string& exe, const string& in) {
    { ofstream f(DIR + "p7990_in.txt", ios::binary); f << in; }
    // ⚠️ 用绝对路径：system() 的工作目录可能不是当前目录，相对路径会「找不到程序」而静默返回 RE
    int rc = system((DIR + exe + " < " + DIR + "p7990_in.txt > " + DIR + "p7990_out.txt").c_str());
    if (rc != 0) return "RE";
    ifstream f(DIR + "p7990_out.txt", ios::binary);
    stringstream ss; ss << f.rdbuf();
    return norm(ss.str());
}

int main(int argc, char** argv) {
    int rounds = (argc > 1) ? atoi(argv[1]) : 3000;
    unsigned seed = (argc > 2) ? (unsigned)atoi(argv[2]) : 1u;
    mt19937 rng(seed);
    int mismU = 0, mismF = 0;
    string firstU, firstUin;
    for (int t = 0; t < rounds; ++t) {
        int style = t % 6;
        int n, q, vmax;
        switch (style) {
            case 0: n = 1 + rng() % 5;  q = 1 + rng() % 8;  vmax = 5;   break; // 小值域，多平局
            case 1: n = 1 + rng() % 5;  q = 1 + rng() % 8;  vmax = 20;  break;
            case 2: n = 1 + rng() % 8;  q = 1 + rng() % 12; vmax = 3;   break;
            case 3: n = 1;              q = 1 + rng() % 6;  vmax = 10;  break; // 只有一个公民
            case 4: n = 1 + rng() % 5;  q = 0;              vmax = 10;  break; // ★ 没有任何事件
            default: n = 1 + rng() % 6; q = 1 + rng() % 10; vmax = 100; break;
        }
        stringstream in;
        in << n << "\n";
        for (int i = 1; i <= n; ++i) in << (rng() % (vmax + 1)) << " \n"[i == n];
        in << q << "\n";
        for (int e = 0; e < q; ++e) {
            // style 5 全是事件2；style 4 没有事件；其余混合
            bool type2;
            if (style == 5) type2 = true;
            else type2 = (rng() % 2 == 0);
            if (type2) {
                in << "2 " << (rng() % (vmax + 1)) << "\n";
            } else {
                in << "1 " << (1 + rng() % n) << " " << (rng() % (vmax + 1)) << "\n";
            }
        }
        string si = in.str();
        string b = runOne("p7990_brute.exe", si);
        string u = runOne("p7990_user.exe", si);
        string f = runOne("p7990_fixed.exe", si);
        if (u != b) { ++mismU; if (mismU == 1) { firstU = "暴力=" + b + " 你的=" + u; firstUin = si; } }
        if (f != b) { ++mismF; if (mismF == 1) printf("修正版也不一致: 暴力=%s 修正=%s\n输入:\n%s\n", b.c_str(), f.c_str(), si.c_str()); }
    }
    printf("对拍 %d 轮（6 种风格）：你的原码不一致 %d 个，修正版不一致 %d 个\n", rounds, mismU, mismF);
    if (mismU) {
        printf("你的原码首个反例：%s\n输入:\n%s\n", firstU.c_str(), firstUin.c_str());
    }
    return (mismU || mismF) ? 7 : 0;
}
