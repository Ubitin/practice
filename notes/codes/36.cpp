// ============================================================
//  test_/36.cpp —— lambda 用法实测
//
//  配套讲义：讲义/lambda函数详解.md
//  另有 test_/36_strictweak.cpp（比较器不满足严格弱序的后果，单独跑）
// ============================================================
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using Clock = chrono::steady_clock;

static void title(const char* s) { printf("\n========== %s ==========\n", s); }
static double ms_since(Clock::time_point t0) {
    return chrono::duration<double, milli>(Clock::now() - t0).count();
}

// 全局变量（lambda 里直接用，不需要捕获）
int gv = 100;
// 静态局部变量：也没有自动存储期，同样不能捕获、但能直接用
int useStaticLocal() {
    static int sv = 7;
    auto f = []() { return sv; };      // 不捕获，直接读
    return f();
}

int main() {
    // ---------------------------------------------------------
    title("[1] 基本语法与返回类型自动推导");
    {
        auto f1 = []() { return 42; };                 // 无参、推导出 int
        auto f2 = [](int a, int b) { return a + b; };  // 两个参数
        auto f3 = [](double x) { return x * 2; };      // 推导出 double
        auto f4 = [](int x) -> ll { return x * 100000000LL; };  // 显式指定返回类型
        printf("  f1() = %d\n", f1());
        printf("  f2(3,4) = %d\n", f2(3, 4));
        printf("  f3(1.5) = %g\n", f3(1.5));
        printf("  f4(12345) = %lld\n", f4(12345));
        printf("  完整形式：[捕获列表](参数列表) -> 返回类型 { 函数体 }\n");
        printf("  能省的都省：无参数可省 ()，返回类型可推导时省 -> T，函数体是单表达式时可省 return\n");
        auto f5 = [](int x) { return x + 1; };
        printf("  f5(1) = %d\n", f5(1));
    }

    // ---------------------------------------------------------
    title("[2] ⭐ 值捕获 = 定义那一刻的【快照】（最容易搞错的一条）");
    {
        int x = 10;
        auto byValue = [x]() { return x; };   // 把 x 拷一份进 lambda
        auto byRef   = [&x]() { return x; };  // 引用外面那个 x
        x = 20;
        printf("  定义时 x=10，之后改成 x=20\n");
        printf("  [x]  值捕获读到的 = %d   ← 还是 10（定义时的快照）\n", byValue());
        printf("  [&x] 引用捕获读到的 = %d   ← 跟着变成 20\n", byRef());
        x = 30;
        printf("  再把 x 改成 30：[x] = %d，[&x] = %d\n", byValue(), byRef());
        printf("  ⚠️ 结论：值捕获是【拷贝进闭包】，之后再改原变量与它无关\n");
    }

    // ---------------------------------------------------------
    title("[3] 捕获列表的 7 种写法");
    {
        int a = 1, b = 2, c = 3;
        auto f1 = []()      { return 0; };            // 不捕获任何东西
        auto f2 = [=]()     { return a + b + c; };    // 全部按值捕获
        auto f3 = [&]()     { return a + b + c; };    // 全部按引用捕获
        auto f4 = [a]()     { return a; };            // 只值捕获 a
        auto f5 = [&a]()    { return a; };            // 只引用捕获 a
        auto f6 = [=, &a]() { return a + b + c; };    // 默认值捕获，但 a 用引用
        auto f7 = [&, a]()  { return a + b + c; };    // 默认引用捕获，但 a 用值
        printf("  []        = %d\n", f1());
        printf("  [=]       = %d\n", f2());
        printf("  [&]       = %d\n", f3());
        printf("  [a]       = %d\n", f4());
        printf("  [&a]      = %d\n", f5());
        printf("  [=, &a]   = %d\n", f6());
        printf("  [&, a]    = %d\n", f7());
        printf("  ⚠️ 混合写法里，第一个位置只能写 = 或 &，之后的必须是具名变量\n");
        printf("  ⚠️ 标准不允许重复（[=, a] / [&, &a] 非法），但 GCC 默认模式会宽容接受；\n");
        printf("     加 -pedantic-errors 才报错：explicit by-copy capture of 'a' redundant with ...\n");
    }

    // ---------------------------------------------------------
    title("[4] mutable：让值捕获的副本可改，且【状态跨调用保留】");
    {
        int cnt = 0;
        auto inc = [cnt]() mutable { return ++cnt; };   // 改的是闭包内的副本
        int r1 = inc(), r2 = inc(), r3 = inc();          // 分成三条语句，顺序确定
        printf("  [cnt]() mutable { return ++cnt; } 依次调用：%d %d %d\n", r1, r2, r3);
        printf("  外面的 cnt 还是 %d  ← mutable 改的是副本，不是原变量\n", cnt);
        printf("  ⚠️ 别写成一个表达式里调多次：printf(\"%%d %%d %%d\", inc(), inc(), inc())\n");
        printf("     函数实参的求值顺序【未指定】，有状态 lambda 尤其危险\n");

        auto incRef = [&cnt]() { return ++cnt; };
        printf("  换成 [&cnt]，调一次后外面 cnt = %d  ← 引用捕获才改得到原变量\n", incRef());
        printf("  → mutable 的意义：给闭包一个「自己的计数器」\n");
    }

    // ---------------------------------------------------------
    title("[5] 全局 / 静态局部变量：不需要捕获，而「捕获」它们是个陷阱");
    {
        auto f = []() { return gv; };                   // 全局：直接读，无需捕获
        printf("  []() 直接读全局 gv = %d（无需捕获）\n", f());
        printf("  读静态局部变量 = %d（同样无需捕获）\n", useStaticLocal());
        printf("  ⚠️ 原因：捕获只能捕获【自动存储期】(automatic storage duration) 的变量；\n");
        printf("     全局变量和 static 变量的生命周期本来就是整个程序，直接访问即可\n");

        // ⭐ 陷阱：对 static 局部变量写「值捕获」，GCC 接受但捕获被完全忽略
        static int sv = 7;
        auto capVal = [sv]() { return sv; };            // 写了 [sv]，以为是快照
        auto noCap  = []() { return sv; };              // 不写捕获
        sv = 99;
        printf("  static sv 从 7 改成 99 后：\n");
        printf("    [sv]() 读到 = %d    noCap() 读到 = %d\n", capVal(), noCap());
        printf("  ⚠️ 两个都是 99 —— 说明 [sv] 这个「值捕获」根本没拷副本，\n");
        printf("     静态变量永远是直接访问，快照语义【不成立】\n");
        printf("  ⚠️ 标准规定不能捕获非自动存储期的变量，GCC 默认只给警告：\n");
        printf("     warning: capture of variable 'sv' with non-automatic storage duration\n");
        printf("     加 -pedantic-errors 才变成 error\n");
    }

    // ---------------------------------------------------------
    title("[6] 泛型 lambda（C++14）：参数写 auto");
    {
        auto add = [](auto x, auto y) { return x + y; };
        printf("  add(3, 4)        = %d\n", add(3, 4));
        printf("  add(1.5, 2.25)   = %g\n", add(1.5, 2.25));
        printf("  add(string,string) = %s\n", add(string("la"), string("mbda")).c_str());
        auto twice = [](const auto& v) { return v + v; };
        printf("  twice(string(\"ab\")) = %s\n", twice(string("ab")).c_str());
        printf("  → 本质是编译器为每种实参类型各生成一个 operator()（模板化的闭包）\n");
    }

    // ---------------------------------------------------------
    title("[7] 递归 lambda：三种写法");
    {
        const int N = 30;
        // ① C++14：把自己当参数传进去（最快，无类型擦除）
        auto fib1 = [](auto&& self, int n) -> ll { return n < 2 ? n : self(self, n - 1) + self(self, n - 2); };
        auto t0 = Clock::now();
        ll r1 = fib1(fib1, N);
        double t1 = ms_since(t0);

        // ② std::function：写法直观，但有类型擦除开销
        function<ll(int)> fib2 = [&](int n) -> ll { return n < 2 ? n : fib2(n - 1) + fib2(n - 2); };
        t0 = Clock::now();
        ll r2 = fib2(N);
        double t2 = ms_since(t0);

        printf("  fib(%d) = %lld / %lld（两种写法结果一致）\n", N, r1, r2);
        printf("  ① 自传参 (auto&& self)  : %8.3f ms\n", t1);
        printf("  ② std::function<ll(int)> : %8.3f ms\n", t2);
        printf("  → 比值 %.1f 倍（std::function 每次调用都要走一次间接跳转）\n", t2 / t1);
        printf("  ⚠️ ① 的坑：必须写 self(self, ...)，只写 self(...) 会编译不过\n");
#ifdef __cpp_explicit_this_parameter
        auto fib3 = [](this auto&& self, int n) -> ll { return n < 2 ? n : self(n - 1) + self(n - 2); };
        printf("  ③ C++23 [] (this auto&& self) 写法可用，fib(%d) = %lld\n", N, fib3(N));
#else
        printf("  ③ C++23 的 (this auto&& self) 写法在当前标准下不可用\n");
#endif
    }

    // ---------------------------------------------------------
    title("[8] ⭐ 同一个 lambda，用在 sort 和 priority_queue 上方向相反");
    {
        vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};

        auto cmp = [](int a, int b) { return a > b; };   // 就这一个 lambda

        vector<int> s = v;
        sort(s.begin(), s.end(), cmp);
        printf("  sort(v, cmp) 结果：");
        for (int x : s) printf("%d ", x);
        printf("  ← 降序（cmp(a,b) 为真表示 a 排前面）\n");

        priority_queue<int, vector<int>, decltype(cmp)> pq(cmp, vector<int>(v.begin(), v.end()));
        printf("  priority_queue 的 top() = %d  ← 堆顶是【最小值】\n", pq.top());
        printf("  → 同一个 cmp：sort 得降序，priority_queue 得小根堆\n");
        printf("  ⚠️ priority_queue 的 cmp 必须【作为构造参数传进去】，不能只写类型\n");
        printf("  ⚠️ 记住口诀：cmp(a,b) = true 表示 a 排在 b【前面】；\n");
        printf("     sort 输出从前往后，priority_queue 的 top() 是「最前面那个」\n");
    }

    // ---------------------------------------------------------
    title("[9] set / map 用 lambda 自定义排序（必须写出类型）");
    {
        auto scmp = [](int a, int b) { return a > b; };
        set<int, decltype(scmp)> s(scmp);            // 降序 set
        for (int x : {3, 1, 4, 1, 5, 9, 2, 6}) s.insert(x);
        printf("  set<int, decltype(scmp)> 遍历：");
        for (int x : s) printf("%d ", x);
        printf("  ← 降序，且自动去重\n");
        printf("  ⚠️ lambda 的类型是匿名的，必须用 decltype(...) 拿到；\n");
        printf("     且要把对象本身传给构造函数 \n");
    }

    // ---------------------------------------------------------
    title("[10] 常见算法的 lambda 用法");
    {
        vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6};
        printf("  count_if 偶数个数        = %lld\n", (long long)count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }));
        auto it = find_if(v.begin(), v.end(), [](int x) { return x > 4; });
        printf("  find_if 第一个 >4 的位置 = 下标 %lld（值 %d）\n", (long long)(it - v.begin()), *it);
        vector<int> t = v;
        t.erase(remove_if(t.begin(), t.end(), [](int x) { return x < 3; }), t.end());
        printf("  remove_if 删掉 <3 后：");
        for (int x : t) printf("%d ", x);
        printf("\n  accumulate 求和          = %d\n", accumulate(v.begin(), v.end(), 0));
        printf("  accumulate 求积(带 lambda) = %d\n",
               accumulate(v.begin(), v.end(), 1, [](int acc, int x) { return acc * x; }));
        nth_element(v.begin(), v.begin() + 2, v.end(), [](int a, int b) { return a > b; });
        printf("  nth_element 按降序取第 3 大 = %d\n", v[2]);
    }

    // ---------------------------------------------------------
    title("[11] 性能：lambda vs std::function（排序 200 万个数）");
    {
        const int N = 2000000;
        mt19937 rng(12345);
        vector<int> base(N);
        for (auto& x : base) x = (int)(rng() % 1000000000);

        vector<int> a = base;
        auto t0 = Clock::now();
        sort(a.begin(), a.end(), [](int x, int y) { return x < y; });
        double tl = ms_since(t0);

        vector<int> b = base;
        function<bool(int, int)> fn = [](int x, int y) { return x < y; };
        t0 = Clock::now();
        sort(b.begin(), b.end(), fn);
        double tf = ms_since(t0);

        printf("  N = %d\n", N);
        printf("  lambda（无捕获，可直接内联） : %8.1f ms\n", tl);
        printf("  std::function               : %8.1f ms\n", tf);
        printf("  → 慢 %.2f 倍；这还只是比较器，递归里用 std::function 会差更多\n", tf / tl);
        printf("  ⚠️ 结论：机试里【别用 std::function 做比较器】，除非要递归\n");
    }

    // ---------------------------------------------------------
    title("[12] 在循环里反复构造 lambda：值捕获大对象 vs 引用捕获");
    {
        vector<int> big(1000000, 0);                 // 4 MB
        const int REP = 200;
        volatile ll sink = 0;
        const ll SZ = (ll)big.size();

        // 每轮先改一下数组内容，保证"拷贝"这件事无法被编译器省掉
        auto t0 = Clock::now();
        for (int i = 0; i < REP; ++i) {
            big[i] = i;
            auto f = [big, i]() { return (ll)big[i]; };   // 值捕获：每次构造拷 4MB
            sink += f();
        }
        double tA = ms_since(t0);

        t0 = Clock::now();
        for (int i = 0; i < REP; ++i) {
            big[i] = i;
            auto f = [&big, i]() { return (ll)big[i]; };  // 引用捕获：只存指针
            sink += f();
        }
        double tB = ms_since(t0);

        printf("  %lld 元素(%.0f MB) 的 vector，各在循环里构造 lambda %d 次：\n",
               SZ, SZ * 4.0 / 1048576.0, REP);
        printf("  [big]  值捕获  : %8.3f ms   ← 每次构造都真拷一份，共 %.2f GB\n",
               tA, REP * SZ * 4.0 / 1e9);
        printf("  [&big] 引用捕获 : %8.3f ms\n", tB);
        if (tB > 0.001) printf("  → 慢 %.0f 倍\n", tA / tB);
        else            printf("  → 引用捕获快到测不出（只存一个指针）\n");
        printf("  ⚠️ 传给 sort 的比较器只构造一次，值捕获无所谓；\n");
        printf("     真正要避免的是【在循环内反复构造值捕获大对象的 lambda】\n");
        (void)sink;
    }

    printf("\n===== 实测结束 =====\n");
    return 0;
}
