// 点灯题：正解 vs 暴力 对拍（自带生成器，风格轮换）
// 用法: lamp_stress.exe [rounds] [seed]
#include <bits/stdc++.h>
using namespace std;

static string runOne(const string& exe, const string& in) {
    // 用临时文件喂输入、收输出（避免管道在 Windows 下的麻烦）
    static int id = 0;
    string fi = "lamp_stress_in.txt", fo = "lamp_stress_out.txt";
    { ofstream f(fi, ios::binary); f << in; }
    string cmd = exe + " < " + fi + " > " + fo;
    int rc = system(cmd.c_str());
    if (rc != 0) return "RE";
    ifstream f(fo, ios::binary);
    stringstream ss; ss << f.rdbuf();
    string s = ss.str();
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r' || s.back() == ' ')) s.pop_back();
    return s;
}

int main(int argc, char** argv) {
    int rounds = (argc > 1) ? atoi(argv[1]) : 500;
    unsigned seed = (argc > 2) ? (unsigned)atoi(argv[2]) : 1u;
    mt19937 rng(seed);
    int mism = 0;
    for (int t = 0; t < rounds; ++t) {
        int style = t % 6;
        int H = 1 + (int)(rng() % 8), W = 1 + (int)(rng() % 8);
        if (style == 1) { H = 1; W = 1 + rng() % 10; }          // 单行
        if (style == 2) { H = 1 + rng() % 10; W = 1; }          // 单列
        if (style == 3) { H = 8; W = 8; }                       // 满尺寸小网格
        int dens;                                                // 障碍密度（百分比）
        switch (style) {
            case 0: dens = 30; break;
            case 1: dens = 50; break;
            case 2: dens = 50; break;
            case 3: dens = 10; break;
            case 4: dens = 90; break;                            // 几乎全障碍
            default: dens = 0; break;                             // 全空地
        }
        stringstream in;
        in << H << " " << W << "\n";
        for (int i = 0; i < H; ++i) {
            for (int j = 0; j < W; ++j) in << (((int)(rng() % 100) < dens) ? '#' : '.');
            in << "\n";
        }
        string si = in.str();
        string a = runOne("lamp_brute.exe", si);
        string b = runOne("lamp_fast.exe", si);
        if (a != b) {
            ++mism;
            if (mism <= 3) {
                printf("不一致 (style=%d): 暴力=%s 正解=%s\n输入:\n%s\n", style, a.c_str(), b.c_str(), si.c_str());
            }
        }
    }
    printf("对拍 %d 轮（6 种风格轮换：随机/单行/单列/8x8稀疏/几乎全障碍/全空地）：不一致 %d 个 %s\n",
           rounds, mism, mism ? "✘" : "✅");
    return mism ? 7 : 0;
}
