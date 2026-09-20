// ============================================================
//  01_p1024_check.cpp —— P1024 一元三次方程求解：漏根原因定位
//
//  原文件 test_/01.cpp 保持原样、未做任何改动。
//
//  原码（逐字复刻）：
//      for(int i=-100;i<=100;++i){
//          double l=i, r=i+1;
//          if(fabs(f(l)) < eps)        printf("%.2lf ", l);      // 只在这里输出
//          else if(fabs(f(r)) < eps)   continue;
//          else if(f(l)*f(r) < 0){
//              while(abs(l-r) > eps){ ...二分... }                // ← 没有输出！
//          }
//      }
// ============================================================
#include <bits/stdc++.h>
using namespace std;

static double A, B, C, D;
static double f(double x) { return A * x * x * x + B * x * x + C * x + D; }

// ---------- 是否模拟"在二分分支后补上输出" ----------
static bool g_printBisection = false;

// 逐字复刻原码逻辑，把结果收集起来（而不是打印）
static vector<double> solve_user(double a, double b, double c, double d) {
    const double eps = 1e-4;
    A = a; B = b; C = c; D = d;
    vector<double> res;
    for (int i = -100; i <= 100; ++i) {
        double l = i, r = i + 1;
        if (fabs(f(l)) < eps) {
            res.push_back(l);
        } else if (fabs(f(r)) < eps) {
            continue;
        } else if (f(l) * f(r) < 0) {
            while (abs(l - r) > eps) {
                double mid = (l + r) / 2;
                if (f(mid) * f(r) > 0) r = mid;
                else l = mid;
            }
            if (g_printBisection) res.push_back((l + r) / 2);   // 原码缺的就是这一句
        }
    }
    return res;
}

static bool sameRounded(const vector<double>& got, const vector<double>& exp) {
    if (got.size() != exp.size()) return false;
    for (size_t i = 0; i < got.size(); ++i)
        if (fabs(got[i] - exp[i]) > 5e-3) return false;   // 2 位小数的容差
    return true;
}

static void show(const char* tag, const vector<double>& v) {
    printf("  %-10s 共 %zu 个: ", tag, v.size());
    for (double x : v) printf("%.2f ", x);
    printf("\n");
}

// ---------- 修正版算法（供对拍） ----------
static vector<double> solve_fixed(double a, double b, double c, double d) {
    A = a; B = b; C = c; D = d;
    vector<double> res;
    auto emit = [&](double x) {
        if (res.empty() || x - res.back() >= 0.5) res.push_back(x);
    };
    for (int i = -100; i < 100; ++i) {
        double l = i, r = i + 1;
        double fl = f(l), fr = f(r);
        if (fl == 0.0) { emit(l); continue; }
        if (fl * fr < 0) {
            for (int k = 0; k < 100; ++k) {
                double mid = (l + r) / 2;
                if (f(mid) * fr > 0) r = mid; else l = mid;
            }
            emit((l + r) / 2);
        }
    }
    if (f(100.0) == 0.0) emit(100.0);
    return res;
}

int main() {
    cout << "===== [1] abs(l - r) 到底调用了哪个重载？ =====\n";
    {
        double x = 0.4;
        printf("  abs(0.4) = %g", (double)abs(x));
        printf("   ← 若是 0 说明走了【整数版】(截断)，若是 0.4 说明走了 double 版\n");
        printf("  sizeof(abs(0.4)) 结果类型判断：%s\n",
               sizeof(abs(x)) == sizeof(int) ? "int（整数版，有截断）" : "double（正确）");
    }

    cout << "\n===== [2] 反例：x^3 - 2x = 0（根为 -1.41, 0.00, 1.41）=====\n";
    {
        vector<double> pureI = {-sqrt(2.0), 0.0, sqrt(2.0)};
        g_printBisection = false;
        auto got = solve_user(1, 0, -2, 0);
        printf("  原码输出：\n");
        show("实际", got);
        show("应有", pureI);
        printf("  → 少了 %zu 个根\n", pureI.size() - got.size());

        g_printBisection = true;
        auto got2 = solve_user(1, 0, -2, 0);
        printf("  只在二分分支后补一句 printf 后：\n");
        show("实际", got2);
        printf("  → 补齐了吗：%s\n", sameRounded(got2, pureI) ? "是 ✔" : "否");
    }

    cout << "\n===== [3] 题面样例（整数根，原码能过）=====\n";
    {
        g_printBisection = false;
        auto got = solve_user(1, -5, -4, 20);
        vector<double> exp = {-2.0, 2.0, 5.0};
        show("实际", got);
        show("应有", exp);
        printf("  → %s（三个根都是整数，被 fabs(f(l))<eps 分支抓到了）\n",
               sameRounded(got, exp) ? "通过 ✔" : "不通过 ✘");
    }

    cout << "\n===== [4] 随机对拍：1000 组三实根（两两间距 >= 1）=====\n";
    {
        mt19937_64 rng(20260917);
        uniform_real_distribution<double> U(-100.0, 95.0);
        uniform_real_distribution<double> G(1.0, 2.5);

        auto run = [&](bool fixBisection) {
            mt19937_64 r2(20260917);              // 同一批数据
            int missCount = 0, wrongCount = 0, total = 0;
            for (int tc = 0; tc < 1000; ) {
                double r1 = U(r2);
                double g1 = G(r2), g2 = G(r2);
                double rr2 = r1 + g1, rr3 = rr2 + g2;
                if (rr3 > 100.0) continue;
                ++tc; ++total;
                // 由根构造系数：k(x-r1)(x-r2)(x-r3)
                double k = (r2() % 2 ? 1.0 : -1.0) * (1.0 + (double)(r2() % 5));
                double b = -k * (r1 + rr2 + rr3);
                double c =  k * (r1 * rr2 + r1 * rr3 + rr2 * rr3);
                double d = -k * r1 * rr2 * rr3;

                vector<double> exp = {r1, rr2, rr3};
                g_printBisection = fixBisection;
                auto got = solve_user(k, b, c, d);
                if (got.size() < 3) missCount++;
                if (!sameRounded(got, exp)) wrongCount++;
            }
            return make_tuple(missCount, wrongCount, total);
        };

        auto [miss1, wrong1, tot1] = run(false);
        printf("  原码（二分分支不输出）：漏根 %d / %d 组，结果不符 %d 组\n", miss1, tot1, wrong1);
        auto [miss2, wrong2, tot2] = run(true);
        printf("  补上 printf 之后      ：漏根 %d / %d 组，结果不符 %d 组\n", miss2, tot2, wrong2);
        printf("  → 漏根的绝大部分原因就是「二分算出根却没输出」\n");
    }

    cout << "\n===== [5] eps 判根对系数的尺度敏感（潜在假根/漏根）=====\n";
    {
        // 系数整体缩小 1e-8：f 的值整体变小，fabs(f(l)) < 1e-4 会被大量误触发
        g_printBisection = true;
        auto got = solve_user(1e-8, 0, -2e-8, 0);
        printf("  a=1e-8, b=0, c=-2e-8, d=0（真根 -1.41, 0.00, 1.41）\n");
        printf("  输出共 %zu 个：", got.size());
        for (size_t i = 0; i < got.size() && i < 12; ++i) printf("%.2f ", got[i]);
        if (got.size() > 12) printf("...");
        printf("\n  → fabs(f(l)) < 1e-4 判的是【函数值】不是【到根的距离】，系数变小就误判一堆整数是根\n");
    }
    cout << "\n===== [6] 修正版 vs 期望（同一批 1000 组随机数据）=====\n";
    {
        mt19937_64 rng(20260917);
        uniform_real_distribution<double> U(-100.0, 95.0);
        uniform_real_distribution<double> G(1.0, 2.5);
        int miss = 0, wrong = 0, total = 0;
        for (int tc = 0; tc < 1000; ) {
            double r1 = U(rng), g1 = G(rng), g2 = G(rng);
            double rr2 = r1 + g1, rr3 = rr2 + g2;
            if (rr3 > 100.0) continue;
            ++tc; ++total;
            double k = (rng() % 2 ? 1.0 : -1.0) * (1.0 + (double)(rng() % 5));
            double b = -k * (r1 + rr2 + rr3);
            double c =  k * (r1 * rr2 + r1 * rr3 + rr2 * rr3);
            double d = -k * r1 * rr2 * rr3;
            vector<double> exp = {r1, rr2, rr3};
            auto got = solve_fixed(k, b, c, d);
            if (got.size() < 3) miss++;
            if (!sameRounded(got, exp)) wrong++;
        }
        printf("  漏根 %d / %d 组，结果不符 %d 组  → %s\n",
               miss, total, wrong, (miss == 0 && wrong == 0) ? "修正版正确 ✔" : "仍有问题 ✘");
    }

    cout << "\n===== [7] 系数整体缩放，修正版稳不稳（根不变，只乘 k）=====\n";
    {
        // 真实根：-2, 2, 5；系数按 k 缩放
        double r1 = -2, r2 = 2, r3 = 5;
        printf("  根恒为 -2.00 2.00 5.00，系数乘不同 k：\n");
        double ks[] = {1e-8, 1e-3, 1, 1e3, 1e6, 1e9};
        for (double k : ks) {
            double b = -k * (r1 + r2 + r3);
            double c =  k * (r1 * r2 + r1 * r3 + r2 * r3);
            double d = -k * r1 * r2 * r3;

            g_printBisection = true;                  // 给原码也补上输出，只比"判根"这一步
            auto ou = solve_user(k, b, c, d);
            auto fx = solve_fixed(k, b, c, d);
            printf("  k=%-8g  原码(补输出后): %2zu 个   修正版: %zu 个 %s\n",
                   k, ou.size(), fx.size(),
                   (fx.size() == 3) ? "✔" : "✘");
        }
        printf("  → 原码在 k 很小时会把一大堆整数点误判成根（fabs(f)<1e-4 是绝对值判据）\n");
    }

    return 0;
}
