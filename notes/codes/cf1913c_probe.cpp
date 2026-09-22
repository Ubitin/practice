// 穷举扫描：把"多重集当成一个和"这个写法，在所有小规模上错成什么样
// 对每个多重集（c0 个 1、c1 个 2、c2 个 4、c3 个 8、c4 个 16）
//   · 真值：w 能否由子集凑出（子集和 DP）
//   · 你的写法：(w & S) == w，其中 S = 总和
// 统计两个方向的错误，并给出最小反例
#include <bits/stdc++.h>
using namespace std;

int main() {
    long long wrongNo = 0, wrongYes = 0;           // 错成 NO / 错成 YES
    long long total = 0;
    string bestNo, bestYes;

    for (int c0 = 0; c0 <= 5; ++c0)
    for (int c1 = 0; c1 <= 5; ++c1)
    for (int c2 = 0; c2 <= 5; ++c2)
    for (int c3 = 0; c3 <= 5; ++c3)
    for (int c4 = 0; c4 <= 5; ++c4) {
        vector<int> vals;
        for (int k = 0; k < c0; ++k) vals.push_back(1);
        for (int k = 0; k < c1; ++k) vals.push_back(2);
        for (int k = 0; k < c2; ++k) vals.push_back(4);
        for (int k = 0; k < c3; ++k) vals.push_back(8);
        for (int k = 0; k < c4; ++k) vals.push_back(16);
        if (vals.empty()) continue;

        long long S = 0;
        for (int x : vals) S += x;

        const int CAP = 128;
        bitset<CAP + 1> dp; dp.reset(); dp[0] = 1;
        for (int x : vals) if (x <= CAP) dp |= (dp << x);

        for (int w = 1; w <= min<long long>(S, CAP); ++w) {
            ++total;
            bool truth = dp.test(w);            // 真正的答案
            bool yours = ((w & S) == w);        // 你的写法
            if (truth == yours) continue;
            ostringstream os;
            os << "多重集{";
            for (size_t i = 0; i < vals.size(); ++i) os << (i ? "," : "") << vals[i];
            os << "} 总和=" << S << "  查 w=" << w
               << "  真值=" << (truth ? "YES" : "NO")
               << "  你的写法=" << (yours ? "YES" : "NO");
            if (truth && !yours) { ++wrongNo; if (bestNo.empty()) bestNo = os.str(); }
            else                 { ++wrongYes; if (bestYes.empty()) bestYes = os.str(); }
        }
    }

    printf("穷举范围：1/2/4/8/16 各 0..5 个，全部 w\n");
    printf("  总 (多重集, w) 组合数 = %lld\n", total);
    printf("  ✘ 真值 YES 但你输出 NO（漏判）  = %lld\n", wrongNo);
    printf("  ✘ 真值 NO  但你输出 YES（多判）  = %lld\n", wrongYes);
    printf("\n最小『漏判』反例：\n  %s\n", bestNo.empty() ? "（无）" : bestNo.c_str());
    printf("\n最小『多判』反例：\n  %s\n", bestYes.empty() ? "（无）" : bestYes.c_str());
    return 0;
}
