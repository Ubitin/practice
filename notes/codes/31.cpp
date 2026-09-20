// P1164 小A点菜 · 0/1 背包计数 —— 终端逐帧动画
// 编译: g++ -std=c++17 -O2 -o 31.exe 31.cpp        运行: 31.exe [fast]
// （fast 参数 = 不停顿，用于快速自测）
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

const int N = 4, M = 4;
const int A[N] = {1, 1, 2, 2};          // 四道菜的价格

int delayMs = 700;

void cls()  { cout << "\033[2J\033[H"; }         // 清屏 + 光标回左上
void col(int c) { cout << "\033[" << c << "m"; }
void rst()  { cout << "\033[0m"; }
void sleep_ms(int ms) { if (ms > 0) this_thread::sleep_for(milliseconds(ms)); }

// 画 dp 数组：src / dst 两个格子高亮
void printDp(const string& label, const vector<int>& dp, int src, int dst, const string& note) {
    cout << label << "\n  ";
    for (int j = 0; j <= M; ++j) {
        string cell = "dp[" + to_string(j) + "]=";
        if (j == dst)      { col(1); col(43); col(30); }   // 目标：黄底黑字加粗
        else if (j == src) { col(1); col(44); col(97); }   // 来源：蓝底白字加粗
        cout << cell;
        printf("%-4d", dp[j]);
        rst();
        cout << "  ";
    }
    cout << "\n";
    if (!note.empty()) cout << "  " << note << "\n";
}

// 完整演示一遍某种遍历顺序，返回最终 dp
vector<int> runOrder(const string& order, bool animate) {
    vector<int> dp(M + 1, 0);
    dp[0] = 1;
    const bool desc = (order == "desc");

    for (int i = 0; i < N; ++i) {
        int x = A[i];
        for (int t = 0; t <= M - x; ++t) {
            int j = desc ? (M - t) : (x + t);          // 倒序 or 正序
            if (animate) {
                cls();
                cout << "=== P1164 小A点菜 · 0/1 背包计数 ===\n";
                cout << "价格: ";
                for (int k = 0; k < N; ++k) {
                    if (k == i) { col(1); col(42); col(30); }
                    cout << "[" << A[k] << "]";
                    rst(); cout << " ";
                }
                cout << "   ← 正在处理第 " << i + 1 << " 道菜（价格 " << x << "）\n\n";
                cout << "遍历顺序: " << (desc ? "倒序 j = m → x（正确）" : "正序 j = x → m（错误）")
                     << "   本道菜第 " << t + 1 << "/" << M - x + 1 << " 步\n\n";
                printDp("改动前:", dp, j - x, j,
                        "dp[" + to_string(j) + "] += dp[" + to_string(j - x) + "]"
                        + "   (" + to_string(dp[j]) + " + " + to_string(dp[j - x]) + ")");
                sleep_ms(delayMs);
            }
            dp[j] += dp[j - x];
            if (animate) {
                printDp("\n改动后:", dp, j - x, j,
                        desc ? "  ✔ dp[" + to_string(j - x) + "] 是「没用过这道菜」的旧值"
                             : "  ✘ dp[" + to_string(j - x) + "] 本轮已被更新 → 这道菜被用了多次");
                sleep_ms(delayMs);
            }
        }
    }
    return dp;
}

int main(int argc, char** argv) {
    if (argc > 1 && string(argv[1]) == "fast") delayMs = 0;

    cls();
    cout << "P1164 小A点菜 —— 题意\n";
    cout << "  4 道菜, 价格 1 1 2 2;口袋 4 元;每道菜最多点一次;求恰好花完 4 元的点法数。\n";
    cout << "  3 种点法: {1,1,2(第3道)}, {1,1,2(第4道)}, {2(第3道),2(第4道)}\n";
    sleep_ms(delayMs * 3);

    vector<int> d1 = runOrder("desc", true);
    cls();
    cout << "=== 倒序（正确写法）跑完了 ===\n\n";
    printDp("最终 dp:", d1, -1, M, "答案 dp[4] = 3  ✔ 与枚举一致");
    sleep_ms(delayMs * 3);

    vector<int> d2 = runOrder("asc", true);
    cls();
    cout << "=== 正序（错误写法）跑完了 ===\n\n";
    printDp("最终 dp:", d2, -1, M, "答案 dp[4] = 14  ✘ 同一道菜被重复使用（退化成完全背包）");
    sleep_ms(delayMs * 2);

    cout << "\n对照表:\n";
    cout << "  遍历顺序            dp[4]   结论\n";
    cout << "  j = m → x (倒序)      " << d1[M] << "     正确：每道菜只用一次\n";
    cout << "  j = x → m (正序)      " << d2[M] << "    错误：dp[j-x] 已被本轮污染\n";
    cout << "\n一句话: dp[0] = 1（什么都不点）, dp[j] += dp[j-x] 必须**倒序**遍历 j。\n";
    return 0;
}
