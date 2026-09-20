// lower_bound / upper_bound 详解 —— 演示程序（配套讲义：lower_bound与upper_bound详解.md）
// 编译: g++ -std=c++17 -O2 -o 32.exe 32.cpp
// 注意: 本机 g++ 按 GBK 解码源文件，字符串字面量里不要放中文（注释可以）
#include <bits/stdc++.h>
using namespace std;
using Clock = chrono::steady_clock;

string fmt(const vector<int>& v, vector<int>::iterator it) {
    if (it == v.end()) return "end()";
    return "pos" + to_string(it - v.begin()) + " val" + to_string(*it);
}
double ms(Clock::time_point a, Clock::time_point b) {
    return chrono::duration<double, milli>(b - a).count();
}

int main() {
    vector<int> v = {1, 3, 3, 3, 7};

    cout << "=== (1) basic table, v = 1 3 3 3 7 ===\n";
    cout << "x    lower_bound(>=x)     upper_bound(>x)      count = ub-lb\n";
    for (int x : {0, 1, 3, 5, 7, 9}) {
        auto lo = lower_bound(v.begin(), v.end(), x);
        auto up = upper_bound(v.begin(), v.end(), x);
        printf("%-4d %-20s %-20s %d\n", x, fmt(v, lo).c_str(), fmt(v, up).c_str(),
               int(up - lo));
    }

    cout << "\n=== (2) four queries ===\n";
    for (int x : {0, 3, 5, 9}) {
        auto lo = lower_bound(v.begin(), v.end(), x);      // 第一个 >= x
        auto up = upper_bound(v.begin(), v.end(), x);      // 第一个 >  x
        string firstGe = fmt(v, lo), firstGt = fmt(v, up);
        string lastLt  = (lo == v.begin()) ? "none" : fmt(v, prev(lo));
        string lastLe  = (up == v.begin()) ? "none" : fmt(v, prev(up));
        printf("x=%-3d first>=: %-10s first>: %-10s last<: %-10s last<=: %-10s\n",
               x, firstGe.c_str(), firstGt.c_str(), lastLt.c_str(), lastLe.c_str());
    }

    cout << "\n=== (3) edge cases (all return end() / begin()) ===\n";
    {
        vector<int> e;                                     // 空容器
        cout << "empty:      lower_bound(1)==end? " << (lower_bound(e.begin(), e.end(), 1) == e.end())
             << "  (此时 begin()==end() 也成立: "
             << (lower_bound(e.begin(), e.end(), 1) == e.begin()) << ")\n";
        vector<int> hi = {10, 20, 30};                     // x 比所有元素都小
        cout << "all > x:    lower_bound(5)==begin? " << (lower_bound(hi.begin(), hi.end(), 5) == hi.begin())
             << "   upper_bound(5)==begin? " << (upper_bound(hi.begin(), hi.end(), 5) == hi.begin()) << "\n";
        vector<int> lo2 = {1, 2, 3};                       // x 比所有元素都大
        cout << "all < x:    lower_bound(9)==end?   " << (lower_bound(lo2.begin(), lo2.end(), 9) == lo2.end())
             << "   upper_bound(9)==end?   " << (upper_bound(lo2.begin(), lo2.end(), 9) == lo2.end()) << "\n";
    }

    cout << "\n=== (4) pos = it - begin() can be n ===\n";
    {
        auto it = lower_bound(v.begin(), v.end(), 9);
        int pos = int(it - v.begin());
        cout << "x=9 -> pos = " << pos << " (v.size() = " << v.size()
             << ")  => pos is a valid INSERT position but v[pos] is out of bounds\n";
    }

    cout << "\n=== (5) existence: binary_search vs lower_bound ===\n";
    for (int x : {3, 4}) {
        cout << "x=" << x << ": binary_search=" << binary_search(v.begin(), v.end(), x);
        auto it = lower_bound(v.begin(), v.end(), x);
        cout << "   lower_bound+check=" << (it != v.end() && *it == x) << "\n";
    }

    cout << "\n=== (6) neighbor candidates: {it, it==begin()? end() : prev(it)} ===\n";
    vector<long long> k = {-9, -4, -1, 2, 3, 7, 12, 15, 21, 30};
    for (long long b : {0LL, -100LL, 100LL, 15LL, -9LL}) {
        auto it = lower_bound(k.begin(), k.end(), b);
        cout << "b=" << setw(5) << b << "  pos=" << setw(2) << (it - k.begin()) << "  cands: ";
        for (auto cand : {it, it == k.begin() ? k.end() : prev(it)}) {
            if (cand == k.end()) { cout << "[skipped] "; continue; }
            cout << *cand << " ";
        }
        cout << "\n";
    }

    cout << "\n=== (7) set/map member version vs global (n=200000, q=2000) ===\n";
    {
        set<int> s;
        for (int i = 0; i < 200000; ++i) s.insert(i * 2);
        mt19937 rng(1);
        vector<int> qs(2000);
        for (auto& x : qs) x = int(rng() % 400000);

        auto t0 = Clock::now();
        long long sum1 = 0;
        for (int x : qs) { auto it = s.lower_bound(x); if (it != s.end()) sum1 += *it; }
        auto t1 = Clock::now();
        long long sum2 = 0;
        for (int x : qs) { auto it = lower_bound(s.begin(), s.end(), x); if (it != s.end()) sum2 += *it; }
        auto t2 = Clock::now();

        printf("member  s.lower_bound(x)      : %8.2f ms  (sum=%lld)\n", ms(t0, t1), sum1);
        printf("global  lower_bound(s.begin..) : %8.2f ms  (sum=%lld)  <- O(n) per query!\n",
               ms(t1, t2), sum2);
        printf("same results? %s\n", sum1 == sum2 ? "yes" : "no");
    }
    return 0;
}
