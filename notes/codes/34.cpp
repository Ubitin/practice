// ============================================================
//  test_/34.cpp  —— 反三角函数（asin / acos / atan / atan2）实测程序
//  配套讲义：讲义/反三角函数详解-asin-acos-atan-atan2.md
//  说明：本文件为新增演示程序，不改动任何既有文件。
//  编译：g++ -std=gnu++17 -O2 34.cpp -o 34
// ============================================================
#include <bits/stdc++.h>
using namespace std;

static void title(const char* s) { printf("\n========== %s ==========\n", s); }

int main() {
    printf("sizeof(double) = %zu\n", sizeof(double));

    // ---------------------------------------------------------
    title("[1] 四个函数的基本值（返回弧度）");
    printf("asin(0.0)   = %.15f\n", asin(0.0));
    printf("asin(0.5)   = %.15f   （应为 pi/6 = %.15f）\n", asin(0.5), acos(-1.0) / 6);
    printf("asin(1.0)   = %.15f   （应为 pi/2 = %.15f）\n", asin(1.0), acos(-1.0) / 2);
    printf("acos(1.0)   = %.15f\n", acos(1.0));
    printf("acos(0.0)   = %.15f\n", acos(0.0));
    printf("acos(-1.0)  = %.15f\n", acos(-1.0));
    printf("atan(1.0)   = %.15f   （应为 pi/4 = %.15f）\n", atan(1.0), acos(-1.0) / 4);
    printf("atan2(1, 1) = %.15f\n", atan2(1.0, 1.0));

    // ---------------------------------------------------------
    title("[2] 值域实测（逼近边界）");
    printf("asin( 1-1e-16) = %.17g\n", asin(1.0 - 1e-16));
    printf("asin( 1.0)     = %.17g\n", asin(1.0));
    printf("acos(-1-1e-16) = %.17g  <-- 越界\n", acos(-1.0 - 1e-16));
    printf("atan(1e300)    = %.17g   （应趋近 pi/2 = %.17g）\n", atan(1e300), acos(-1.0) / 2);
    printf("atan(-1e300)   = %.17g\n", atan(-1e300));

    // ---------------------------------------------------------
    title("[3] 定义域错误 → NaN（不报错、不抛异常）");
    double bad1 = asin(2.0), bad2 = acos(-2.0), bad3 = sqrt(-1.0);
    printf("asin(2.0)   = %f   isnan=%d\n", bad1, (int)isnan(bad1));
    printf("acos(-2.0)  = %f   isnan=%d\n", bad2, (int)isnan(bad2));
    printf("sqrt(-1.0)  = %f   isnan=%d\n", bad3, (int)isnan(bad3));
    printf("atan(2.0)   = %f   （atan 无定义域限制，任意实数都有值）\n", atan(2.0));
    printf("NaN == NaN ？ %d    （NaN 与任何值比较都是 false）\n", (int)(bad1 == bad1));
    printf("bad1 < 0 ？%d   bad1 > 0 ？%d   （NaN 比较全为 false，会静默走错分支）\n",
           (int)(bad1 < 0), (int)(bad1 > 0));

    // ---------------------------------------------------------
    title("[4] atan vs atan2：象限能力");
    double xs[] = {1, -1, -1, 1, 0, 0};
    double ys[] = {1, 1, -1, -1, 1, -1};
    printf("%-10s %-24s %-24s\n", "(x,y)", "atan(y/x)  [LOST]", "atan2(y,x) [OK]");
    for (int i = 0; i < 6; i++) {
        double x = xs[i], y = ys[i];
        char buf1[64], buf2[64];
        if (x == 0) snprintf(buf1, sizeof buf1, "除零 → inf/-inf/NaN");
        else        snprintf(buf1, sizeof buf1, "%.6f", atan(y / x));
        snprintf(buf2, sizeof buf2, "%.6f", atan2(y, x));
        printf("(%4.0f,%4.0f)  %-24s %-24s\n", x, y, buf1, buf2);
    }
    printf("⚠️ 注意参数顺序：atan2(y, x) —— y 在前！\n");
    printf("atan2(1,0) = %.6f (pi/2)   atan2(0,1) = %.6f (0)\n", atan2(1.0, 0.0), atan2(0.0, 1.0));
    printf("atan2(0,0) = %.6f  （(0,0) 无定义，标准规定返回 0）\n", atan2(0.0, 0.0));

    // ---------------------------------------------------------
    title("[5] 弧度 ↔ 角度");
    const double PI = acos(-1.0);
    double deg = 30.0;
    printf("30 度 = %.15f 弧度\n", deg * PI / 180.0);
    printf("回算   = %.15f 度\n", (deg * PI / 180.0) * 180.0 / PI);
    printf("asin(0.5) 转角度 = %.10f 度\n", asin(0.5) * 180.0 / PI);
    printf("[注意] 反三角函数返回的是【弧度】，不是角度！\n");

    // ---------------------------------------------------------
    title("[6] 夹角精度：acos 公式 vs atan2 公式（极小夹角）");
    printf("%-10s %-22s %-22s %-22s\n", "真实 theta", "acos(dot/(|a||b|))", "atan2(cross,dot)", "acos 相对误差");
    for (int k = 1; k <= 16; k++) {
        double th = pow(10.0, -k);
        double bx = cos(th), by = sin(th);
        // a=(1,0), b=(bx,by)
        double dot = 1.0 * bx + 0.0 * by;
        double crs = 1.0 * by - 0.0 * bx;
        double na = 1.0, nb = sqrt(bx * bx + by * by);
        double ratio = dot / (na * nb);
        double ang_acos = acos(ratio);
        double ang_atan2 = atan2(crs, dot);
        double relerr = (th > 0) ? fabs(ang_acos - th) / th : 0.0;
        char rb[32];
        if (ratio > 1.0) snprintf(rb, sizeof rb, "ratio>1!");
        else             snprintf(rb, sizeof rb, "%.3e", relerr);
        printf("1e-%-7d %-22.17g %-22.17g %-22s\n", k, ang_acos, ang_atan2, rb);
    }

    // ---------------------------------------------------------
    title("[7] 夹角精度：接近 pi 的情形");
    printf("%-10s %-22s %-22s\n", "真实 theta", "acos 公式", "atan2 公式");
    for (int k = 1; k <= 12; k++) {
        double eps = pow(10.0, -k);
        double th = PI - eps;                 // 真实夹角接近 pi
        double bx = cos(th), by = sin(th);    // by 很小
        double dot = bx, crs = by;
        double ratio = dot / sqrt(bx * bx + by * by);
        printf("pi-1e-%-5d %-22.17g %-22.17g\n", k, acos(ratio), atan2(crs, dot));
    }

    // ---------------------------------------------------------
    title("[8] acos 公式的 ratio 越界（→ NaN）—— 必须用「近平行/近反平行」样本才搜得到");
    {
        mt19937_64 rng(20260917);
        uniform_real_distribution<double> U(-1e6, 1e6);

        // (a) 两个独立随机向量：夹角均匀分布，ratio 几乎不会落在 ±1 附近
        long long badA = 0;
        const long long TOTA = 2000000;
        for (long long i = 0; i < TOTA; i++) {
            double ax = U(rng), ay = U(rng), bx = U(rng), by = U(rng);
            double na = sqrt(ax * ax + ay * ay), nb = sqrt(bx * bx + by * by);
            if (na == 0 || nb == 0) continue;
            double ratio = (ax * bx + ay * by) / (na * nb);
            if (ratio > 1.0 || ratio < -1.0) badA++;
        }
        printf("(a) 独立随机向量 %lld 组 → ratio 越界 %lld 组\n", TOTA, badA);

        // (b) 近平行 / 近反平行：这才是真实几何题里 ratio 越界的来源
        long long badP = 0, badQ = 0, nanP = 0, nanQ = 0;
        const long long TOTB = 2000000;
        uniform_real_distribution<double> E(6.0, 16.0);   // 扰动 1e-6 ~ 1e-16
        for (long long i = 0; i < TOTB; i++) {
            double ax = U(rng), ay = U(rng);
            double na = sqrt(ax * ax + ay * ay);
            if (na == 0) continue;
            double ux = ax / na, uy = ay / na;
            double s = pow(10.0, -E(rng));
            // 近平行：b ≈ a，单位方向转一个极小角
            double px = ux - uy * s, py = uy + ux * s;
            // 近反平行：b ≈ -a
            double qx = -ux - uy * s, qy = -uy + ux * s;
            double np = sqrt(px * px + py * py), nq = sqrt(qx * qx + qy * qy);
            double rp = (ax * px + ay * py) / (na * np);
            double rq = (ax * qx + ay * qy) / (na * nq);
            if (rp > 1.0 || rp < -1.0) badP++;
            if (rq > 1.0 || rq < -1.0) badQ++;
            if (isnan(acos(rp))) nanP++;
            if (isnan(acos(rq))) nanQ++;
        }
        printf("(b) 近平行   %lld 组 → ratio 越界 %lld 组，其中 acos 得 NaN 共 %lld 组\n", TOTB, badP, nanP);
        printf("(b) 近反平行 %lld 组 → ratio 越界 %lld 组，其中 acos 得 NaN 共 %lld 组\n", TOTB, badQ, nanQ);
        printf("→ 结论：acos 公式不是「偶尔不准」，而是**在近平行/反平行时直接给出 NaN**；\n");
        printf("   而 atan2(cross, dot) 在同样的输入上永远有定义（cross→0 时给出 0 或 pi）。\n");
        // 示范一个能被稳定复现的例子
        double ax = 1.0, ay = 0.0, bx = 1.0, by = 1e-9, n2 = sqrt(1.0 + 1e-18);
        double rr = (ax * bx + ay * by) / (1.0 * n2);
        printf("   例：a=(1,0), b=(1,1e-9) → ratio=%.17g → acos=%.15g，atan2=%.15g\n",
               rr, acos(rr), atan2(ax * by - ay * bx, ax * bx + ay * by));
    }

    // ---------------------------------------------------------
    title("[9] 极角排序：atan2 比较器 vs 半平面+叉积比较器（性能）");
    {
        const int N = 500000;
        mt19937_64 rng(42);
        uniform_int_distribution<int> U(-1000000, 1000000);
        vector<pair<double,double>> pts;
        pts.reserve(N);
        while ((int)pts.size() < N) {
            double x = U(rng), y = U(rng);
            if (x == 0 && y == 0) continue;
            pts.push_back({x, y});
        }

        // ① atan2 比较器
        auto t0 = chrono::steady_clock::now();
        vector<pair<double,double>> a = pts;
        sort(a.begin(), a.end(), [](const pair<double,double>& p, const pair<double,double>& q) {
            return atan2(p.second, p.first) < atan2(q.second, q.first);
        });
        auto t1 = chrono::steady_clock::now();

        // ② 半平面 + 叉积比较器
        auto half = [](const pair<double,double>& p) {
            return (p.second > 0) || (p.second == 0 && p.first > 0);
        };
        auto t2 = chrono::steady_clock::now();
        vector<pair<double,double>> b = pts;
        sort(b.begin(), b.end(), [&](const pair<double,double>& p, const pair<double,double>& q) {
            bool hp = half(p), hq = half(q);
            if (hp != hq) return hp > hq;
            return p.first * q.second - p.second * q.first > 0;   // 叉积 > 0 表示 p 在 q 逆时针方向
        });
        auto t3 = chrono::steady_clock::now();

        double ms1 = chrono::duration<double, milli>(t1 - t0).count();
        double ms2 = chrono::duration<double, milli>(t3 - t2).count();
        printf("N = %d\n", N);
        printf("① atan2 比较器        : %8.1f ms\n", ms1);
        printf("② 半平面+叉积比较器   : %8.1f ms\n", ms2);
        printf("   加速比 = %.1f 倍\n", ms1 / ms2);
        // 校验：两种排序是否给出同一个「循环序」
        // 做法：各自旋转到 atan2 最小的元素开头，再逐项比较
        auto rotToMin = [](const vector<pair<double,double>>& v) {
            int k = 0; double best = 1e18, bk = 0;
            for (int i = 0; i < (int)v.size(); i++) {
                double t = atan2(v[i].second, v[i].first);
                if (t < best) { best = t; k = i; }
            }
            (void)bk;
            vector<pair<double,double>> r(v.begin() + k, v.end());
            r.insert(r.end(), v.begin(), v.begin() + k);
            return r;
        };
        vector<pair<double,double>> ra = rotToMin(a), rb = rotToMin(b);
        long long mismatch = 0;
        for (int i = 0; i < N; i++) if (ra[i] != rb[i]) mismatch++;
        printf("② 与 ① 的排序结果逐项比较（旋转对齐后）：不一致 %lld / %d 项 → %s\n",
               mismatch, N, mismatch == 0 ? "两者完全等价 ✔" : "需检查");
        // 说明：half-plane 版本的首元素是 atan2 最小者，故旋转对齐即可直接比较
    }

    // ---------------------------------------------------------
    title("[10] 自己实现 atan：泰勒级数 vs 参数缩减");
    auto atan_taylor = [](double x, int n) {          // |x| <= 1
        double sum = 0.0, term = x, x2 = x * x;
        for (int i = 0; i < n; i++) {
            double t = term / (2 * i + 1);
            sum += (i % 2 == 0) ? t : -t;
            term *= x2;
        }
        return sum;
    };
    auto atan_reduce = [&](double x, int n, int k) {  // 半角缩减 k 次
        for (int i = 0; i < k; i++) x = x / (1.0 + sqrt(1.0 + x * x));
        return atan_taylor(x, n) * (double)(1 << k);
    };
    printf("%-8s %-16s %-14s %-22s %-14s %-22s\n", "x", "std::atan", "泰勒20项", "泰勒20项误差", "缩减6次+泰勒20", "缩减版误差");
    double tests[] = {0.1, 0.5, 0.9, 1.0};
    for (double x : tests) {
        double ref = atan(x);
        double t20 = atan_taylor(x, 20);
        double r20 = atan_reduce(x, 20, 6);
        printf("%-8.1f %-16.15f %-14.15f %-22.3e %-14.15f %-22.3e\n",
               x, ref, t20, fabs(t20 - ref), r20, fabs(r20 - ref));
    }
    printf("[收敛问题] 泰勒级数在 x=1 处收敛极慢（交替级数，误差 ~ 1/n）：\n");
    for (int n : {10, 100, 1000, 10000, 100000}) {
        printf("  泰勒 %6d 项, x=1 → %.15f  误差 %.3e\n", n, atan_taylor(1.0, n), fabs(atan_taylor(1.0, n) - atan(1.0)));
    }

    // ---------------------------------------------------------
    title("[11] 反双曲函数（C++11，<cmath>）");
    printf("asinh(1.0)  = %.15f\n", asinh(1.0));
    printf("acosh(2.0)  = %.15f\n", acosh(2.0));
    printf("atanh(0.5)  = %.15f   （= 0.5*ln(3) = %.15f）\n", atanh(0.5), 0.5 * log(3.0));
    printf("acosh(0.5)  = %f  isnan=%d  （定义域 x>=1）\n", acosh(0.5), (int)isnan(acosh(0.5)));
    printf("atanh(1.0)  = %f  isinf=%d  （定义域 |x|<1，±1 处发散）\n", atanh(1.0), (int)isinf(atanh(1.0)));

    // ---------------------------------------------------------
    title("[12] 精度自洽性：f(反函数(f(x))) == x ？");
    printf("sin(asin(0.7))      = %.17g\n", sin(asin(0.7)));
    printf("cos(acos(0.7))      = %.17g\n", cos(acos(0.7)));
    printf("tan(atan(0.7))      = %.17g\n", tan(atan(0.7)));
    double x = 0.9;
    printf("atan(sqrt(1-x*x)/x) vs acos(x): %.17g vs %.17g  （差 %.3e）\n",
           atan(sqrt(1 - x * x) / x), acos(x), fabs(atan(sqrt(1 - x * x) / x) - acos(x)));
    printf("asin(x) + acos(x) = %.17g  （应为 pi/2 = %.17g）\n", asin(x) + acos(x), acos(-1.0) / 2);

    printf("\n===== 实测结束 =====\n");
    return 0;
}
