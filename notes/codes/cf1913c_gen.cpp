// CF1913C 随机数据生成器（种子从命令行来，风格轮换）
// 用法: cf1913c_gen.exe <seed>
//
// 风格（seed % 4）：
//   0 纯随机         : x ∈ [0,7]，w 随机
//   1 大量重复       : x ∈ [0,2]，几乎全是重复加入 —— 专杀"把多重集当成一个和"的写法
//   2 只有两种指数   : 反复加同样的两个值
//   3 两份大值       : 先加同一个 2^x 两份，再查 2^x / 2^(x+1) —— 专杀"和丢掉了数量信息"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    long long seed = argc > 1 ? atoll(argv[1]) : 1;
    mt19937_64 rng((unsigned long long)seed * 1000003ULL + 11ULL);
    auto rnd = [&](int hi) { return (int)(rng() % (unsigned long long)(hi + 1)); };
    int style = (int)(seed % 4);

    int q = 1 + rnd(14);
    vector<string> lines;
    lines.push_back(to_string(q));

    int hiX = (style == 0) ? 7 : (style == 1 ? 2 : (style == 2 ? 6 : 8));
    int two1 = rnd(hiX), two2 = rnd(hiX);

    for (int i = 0; i < q; ++i) {
        int kind = rnd(2);   // 0 = 加, 1 = 查
        if (style == 3 && i < 2) kind = 0;  // 先塞两份同样的
        if (kind == 0) {
            int x;
            if (style == 1)      x = rnd(2);
            else if (style == 2) x = (rnd(1) ? two1 : two2);
            else if (style == 3) x = (i == 0 ? hiX : (i == 1 ? hiX : rnd(hiX)));
            else                 x = rnd(hiX);
            lines.push_back("1 " + to_string(x));
        } else {
            long long w;
            if (style == 3) {
                int x = hiX;
                w = (rnd(2) ? (1LL << x) : (1LL << (x + 1)));
            } else {
                int bits = rnd(hiX + 2);
                w = 1 + rnd((int)min<long long>(262143, (1LL << bits) - 1));  // ≤ 262143
            }
            lines.push_back("2 " + to_string(w));
        }
    }

    for (auto& s : lines) printf("%s\n", s.c_str());
    return 0;
}
