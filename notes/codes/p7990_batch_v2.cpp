// 批量对拍器：生成 K 个随机用例合成一个输入文件，三个程序各跑一次，逐行比对
// 用法: p7990_batch.exe [K] [seed]
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(int argc, char** argv) {
    int K = (argc > 1) ? atoi(argv[1]) : 5000;
    unsigned seed = (argc > 2) ? (unsigned)atoi(argv[2]) : 1u;
    mt19937 rng(seed);
    const string DIR = "D:\\lenovo\\chat_with_deepseek_harness\\test_\\";

    stringstream in;
    vector<int> expectN;
    for (int t = 0; t < K; ++t) {
        int style = t % 8;
        int n, q, vmax;
        switch (style) {
            case 0: n = 1 + rng() % 3;  q = 1 + rng() % 6;  vmax = 4;   break; // 极小值域：大量平局
            case 1: n = 1 + rng() % 4;  q = 1 + rng() % 6;  vmax = 30;  break;
            case 2: n = 1;              q = 1 + rng() % 5;  vmax = 10;  break; // 单个公民
            case 3: n = 1 + rng() % 5;  q = 0;              vmax = 10;  break; // ★ 一个事件都没有
            case 4: n = 1 + rng() % 5;  q = 1 + rng() % 6;  vmax = 10;  break; // 全 2 类（下面控制）
            case 5: n = 1 + rng() % 5;  q = 1 + rng() % 6;  vmax = 10;  break; // 全 1 类
            case 6: n = 1 + rng() % 2;  q = 3 + rng() % 5;  vmax = 5;   break;
            default: n = 2 + rng() % 4; q = 1 + rng() % 8;  vmax = 100; break;
        }
        in << n << "\n";
        for (int i = 1; i <= n; ++i) in << (rng() % (vmax + 1)) << " \n"[i == n];
        in << q << "\n";
        for (int e = 0; e < q; ++e) {
            bool t2;
            if (style == 4) t2 = true;              // 全 2 类
            else if (style == 5) t2 = false;        // 全 1 类
            else t2 = (rng() % 2 == 0);
            if (t2) in << "2 " << (rng() % (vmax + 1)) << "\n";
            else    in << "1 " << (1 + rng() % n) << " " << (rng() % (vmax + 1)) << "\n";
        }
        expectN.push_back(n);
    }

    { ofstream f(DIR + "p7990_batch_in.txt", ios::binary); f << in.str(); }

    auto runOne = [&](const string& exe) -> vector<string> {
        system((DIR + exe + " < " + DIR + "p7990_batch_in.txt > " + DIR + "p7990_batch_" + exe + ".out").c_str());
        ifstream f(DIR + "p7990_batch_" + exe + ".out");
        vector<string> lines; string s;
        while (getline(f, s)) {
            while (!s.empty() && (s.back() == '\r' || s.back() == ' ')) s.pop_back();
            if (!s.empty()) lines.push_back(s);
        }
        return lines;
    };

    auto B = runOne("p7990_brute.exe");
    auto U = runOne("p7990_user_v2_batch.exe");
    auto F = runOne("p7990_fixed.exe");

    printf("用例数 = %d（期望输出行数 %d）\n", K, (int)expectN.size());
    printf("暴力解输出行数 = %d %s\n", (int)B.size(), (int)B.size() == K ? "" : "⚠️ 行数不符（可能崩了）");
    if ((int)B.size() != K) return 9;

    int badU = 0, badF = 0, firstU = -1, firstF = -1;
    for (int i = 0; i < K; ++i) {
        if (i < (int)U.size() && U[i] != B[i]) { ++badU; if (firstU < 0) firstU = i; }
        else if (i >= (int)U.size()) { ++badU; if (firstU < 0) firstU = i; }
        if (i < (int)F.size() && F[i] != B[i]) { ++badF; if (firstF < 0) firstF = i; }
        else if (i >= (int)F.size()) { ++badF; if (firstF < 0) firstF = i; }
    }
    printf("你的原码  不一致 %d / %d %s\n", badU, K, badU ? "✘" : "✅");
    printf("修正版    不一致 %d / %d %s\n", badF, K, badF ? "✘" : "✅");
    if (firstU >= 0) printf("  原码首个错处：第 %d 个用例  暴力=[%s]  你的=[%s]\n", firstU + 1,
                            B[firstU].c_str(), firstU < (int)U.size() ? U[firstU].c_str() : "<无输出>");
    if (firstF >= 0) printf("  修正版首个错处：第 %d 个用例  暴力=[%s]  修正=[%s]\n", firstF + 1,
                            B[firstF].c_str(), firstF < (int)F.size() ? F[firstF].c_str() : "<无输出>");
    return (badU || badF) ? 7 : 0;
}
