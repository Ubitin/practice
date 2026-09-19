# 链接错误排查：`undefined reference to 'WinMain'`

> 触发点：`g++ 01.cpp -o 01` 报
> `ld.exe: .../crtexewin.c:(.text.startup+0xc6): undefined reference to 'WinMain'` + `collect2.exe: error: ld returned 1 exit status`
> 一句话：**编译器没问题，是你的 `01.cpp` 里根本没有 `main` 函数。** 这个报错名字很有误导性——它其实是在说"我找不到 `main`"。
> 演示程序：`test_\01_bsanswer_verify.cpp`；修正版：`test_\01_bsanswer_fixed.cpp`（**原文件 `test_\01.cpp` 保持原样未改动**）。

---

## 一、先分清：这是**链接**错误，不是**编译**错误

| | 编译错误 | 链接错误 |
|---|---|---|
| 报错来自 | `cc1plus.exe` | **`ld.exe`** |
| 典型措辞 | `error: expected ';' before ...`（带 `文件:行:列`） | **`undefined reference to ...`** |
| 最后一行 | 只有 error | **`collect2.exe: error: ld returned 1 exit status`** |
| 含义 | 某个文件**语法/语义**不对 | 语法都过了，但**符号对不上** |

看到 `ld.exe` + `undefined reference` + `collect2.exe` 这三件套，就**不要再怀疑编译器或环境了**，直接去查符号。

### ⭐ `undefined reference to X` 的唯一含义

> **有人"引用"了 X，但没有任何地方"定义"X。**

本次是：启动代码 `crtexewin.o` **引用** `WinMain`，而你的文件没**定义** `WinMain`。

**为什么找 `WinMain` 而不是直接说"缺少 main"？**
MinGW 的启动代码同时准备了两个入口：控制台程序用 `main`，Windows 图形程序用 `WinMain`。它先尝试配 `main`；配不上时，链接器只能退而求其次去要 `WinMain`，于是报出这个名字。

> 对照：**MSVC 遇到同样问题会报 `unresolved external symbol _main`**——措辞不同，病因完全一样。别被名字带偏。

## 二、三条命令定位（本机实测）

### ① 复现

```powershell
g++ 01.cpp -o 01
# ld.exe: ... crtexewin.c: undefined reference to `WinMain'
# collect2.exe: error: ld returned 1 exit status
```

### ② 只做语法+语义检查，**不链接**：`-fsyntax-only`

```powershell
g++ -fsyntax-only 01.cpp
```

**本机结果：完全没有输出，退出码 0。**

> **这就是决定性证据**：编译器前端认为这份代码语法语义**毫无问题**。既然语法没错、又链接不了，那问题只能在"符号定义"上。

### ③ 只编译不链接：`-c`，再看符号表

```powershell
g++ -c 01.cpp -o _probe.o
nm _probe.o
```

**本机 `nm` 输出里的两个全局函数符号**：

```
0000000000000000 T _Z1pxSt6vectorIxSaIxEExx        ← 即 p(long long, vector<long long>, long long, long long)
0000000000000113 T _Z4findxSt6vectorIxSaIxEE       ← 即 find(long long, vector<long long>)
```

**没有任何 `main`**。（`T` = 已定义在 `.text` 段的全局符号；`U` = 未定义、需要外部提供。）

> `_Z1p...` / `_Z4find...` 是 C++ 的**名字修饰（mangling）**：`_Z` + 名字长度 + 名字 + 参数类型编码。看符号表时看到 `_Z...` 就知道是 C++ 函数。

**结论**：文件里只有 `p` 和 `find` 两个函数定义，没有 `main` → 链接器找不到入口 → 报 `WinMain`。

## 三、还有哪些情况会报同一个错

| # | 原因 | 怎么确认 |
|---|---|---|
| 1 | **压根没写 `main`**（本次） | `nm *.o` 里找不到 `main`；或 `grep -n main 文件` |
| 2 | `main` 拼错：`mian` / `maim` / `Main` | `-fsyntax-only` 会过（只是定义了个普通函数），`nm` 里看到 `mian` |
| 3 | `main` 被 `/* */` 或 `#if 0` 吞掉 | 检查注释块和条件编译是否配对 |
| 4 | `static int main()` | static 是**内部链接**，链接器看不到 → 去掉 `static` |
| 5 | 编译的是**片段文件**（只写了几个函数） | 确认 `g++` 后面跟的是完整源文件 |
| 6 | 大括号不配对，`main` 被吞进上一个函数体 | 通常会先报语法错，但有时代码恰好合法 |
| 7 | 文件里有**全角字符**（中文括号、全角分号） | 报错行号附近逐字符看 |
| 8 | 故意用 `-mwindows` 做 GUI 程序 | 那种情况确实要写 `WinMain` |

> **最容易被骗的是 #2**：`int mian(){}` 语法完全合法，`-fsyntax-only` 一声不响，只有链接时才炸，而且报的还是 `WinMain`。

## 四、顺带：这份代码还有更严重的逻辑问题

补上 `main` 之后你会发现它还是不对。逐条列出（**都已实测**）：

### ★② 数组从未排序

`p()` 里用了 `upper_bound`，而 **`upper_bound` 要求区间已按升序排列**，否则返回值毫无意义。原码从读入到二分之间没有任何 `sort`。

### ★③ `mid` 算在 `while` 外面 → 死循环

```cpp
long long mid = (l + r) >> 1;      // ← 只算一次
while(l <= r){
    if(p(v[mid], v, h, l)){ ans = mid; r = mid - 1; }
    l = mid + 1;                   // ← 每轮把 l 赋成同一个值
}
```

`mid` 是常量 ⇒ 每轮 `l = mid + 1` 都赋同一个值。只要 `mid + 1 <= r`，**状态永远不变 → 死循环**。

### ★④ `l = mid + 1` 写在 `if` 外面

`true` 分支里刚做了 `r = mid - 1`（该往小走），紧接着又无条件执行 `l = mid + 1`（往大走）——**两个边界同时被改，逻辑自相矛盾**。正确写法是二者互斥。

### ★⑤ 二分的是**下标**，而答案可能不在数组里 —— 最致命

`p(v[mid], ...)` 意味着假定"最优锯片高度一定是某个树高"。**这个假定是错的。**

**反例（实测）**：`a = {20, 15}`，`M = 7`

| H | W(H) = Σmax(0, hᵢ−H) | 是否 ≥ M |
|---|---|---|
| 17 | 3 | ✘ |
| 16 | 4 | ✘ |
| 15 | 5 | ✘ |
| **14** | **7** | **✔** |
| 13 | 9 | ✔ |

最大可行 `H = 14`，而 **14 不在 {20, 15} 里**。

本机实测结果：

```
值域二分得 14（✔）   下标域二分得 0（✘）
```

**随机数据统计**（30000 组，树高 ≤ 40）：

```
值域二分     错     0 组
下标域二分   错 16375 组        （54.6%）
真答案【不在】树高数组里的情况：28502 组（占 95.0%）
```

> **95% 的随机数据，答案都不在数组里。** 下标域二分一碰上就必然 WA。

**⚠️ 最阴险的地方**：P1873 的两个官方样例答案（15 和 36）**恰好都在数组里**，所以下标域二分能"蒙对"样例：

```
输入 4 7 / 20 15 10 17 → 期望 15，下标域二分也得到 15
```

**样例过了，交上去照样 WA。** 这是最典型的"样例依赖型错误"。

### ☆⑥ `p()` 里把 `l` 当区间左端点

```cpp
auto iter = upper_bound(v.begin() + l, v.end(), x);
```

木材总量必须统计**所有**高于 `H` 的树。因为 `l` 在循环里被改，当 `l > mid` 时，`(mid, l)` 之间的树会被跳过 → 少算木材。

> 其实这里 `l` 参数是多余的：`x = v[mid]`，所有值大于 `x` 的元素下标必然 `> mid`，直接从 `v.begin()` 或 `v.begin()+mid` 起搜就对。

### ☆⑦ 两个函数都按值传递 vector

```cpp
bool p(long long x, vector<long long> v, ...)       // ❌ 每次调用 O(n) 拷贝
long long find(long long h, vector<long long> v)    // ❌ 同上
```

### ☆⑧ `cout << ans` 写在 `find()` 里，函数却 `return 0`

输出应该是 `main` 的职责；而且函数名 `find` 会**遮蔽 `std::find`**，容易踩坑。

## 五、正确写法：二分**值域**，mid 在循环内算

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll n, m;
vector<ll> a;

bool ok(ll H) {                       // 锯片高度 H 能否得到 >= m 的木材
    ll sum = 0;
    for (ll x : a) {
        if (x > H) {
            sum += x - H;
            if (sum >= m) return true;   // 提前返回：既快又防溢出
        }
    }
    return sum >= m;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    if (!(cin >> n >> m)) return 0;
    a.resize(n);
    for (ll i = 0; i < n; ++i) cin >> a[i];

    ll lo = 0, hi = *max_element(a.begin(), a.end());   // ★ 二分【值域】，不是下标
    while (lo < hi) {
        ll mid = lo + (hi - lo + 1) / 2;   // ★ mid 在循环内；+1 上取整防死循环
        if (ok(mid)) lo = mid;             // 可行 → 试更大的
        else         hi = mid - 1;
    }
    cout << lo << '\n';
    return 0;
}
```

**回归测试（实测）**：

| 输入 | 期望 | 实得 |
|---|---|---|
| `4 7` / `20 15 10 17` | 15 | **15** ✔ |
| `5 20` / `4 42 40 26 46` | 36 | **36** ✔ |
| `2 7` / `20 15` | 14 | **14** ✔ |

## 六、二分答案的两套模板 + 三条铁律

```cpp
// 【找最后一个满足 check 的值】（求最大值用它）
ll lo = 最小值, hi = 最大值;
while (lo < hi) {
    ll mid = lo + (hi - lo + 1) / 2;   // ⚠️ 上取整
    if (check(mid)) lo = mid;
    else            hi = mid - 1;
}
cout << lo;

// 【找第一个满足 check 的值】（求最小值用它）
ll lo = 最小值, hi = 最大值;
while (lo < hi) {
    ll mid = lo + (hi - lo) / 2;       // ⚠️ 下取整
    if (check(mid)) hi = mid;
    else            lo = mid + 1;
}
cout << lo;
```

### ⚠️ 三条铁律

1. **`mid` 必须在循环体内计算。** 写在外面 = 死循环（本次原码就犯了这个）。
2. **上取整模板配 `lo = mid`，下取整模板配 `hi = mid`。** 配错就死循环。
   - 记忆法：让"不动的那个边界"决定了取整方向。`lo = mid` 时若下取整，`lo` 可能原地不动 → 死循环。
3. **`check` 的二分域是"答案的取值范围"，不是数组下标。** 答案往往不是输入里的某个数（本次实测 95% 的情况如此）。

## 七、坑清单

| # | 坑 | 症状 |
|---|---|---|
| 1 | 没写 `main` | `undefined reference to 'WinMain'` |
| 2 | `main` 拼成 `mian` | 同上，且 `-fsyntax-only` 不报错 |
| 3 | 看到 `WinMain` 就去重装编译器 | 白折腾；根本不是环境问题 |
| 4 | `mid` 写在 `while` 外面 | 死循环（TLE） |
| 5 | `l = mid+1` 写在 `if` 外面 | 两边界同时改，结果随机 |
| 6 | 二分下标而非值域 | **样例能过，评测 WA**（实测 95% 的数据会错） |
| 7 | 只靠样例验证 | P1873 两个官方样例都"恰好"能被下标域二分蒙对 |
| 8 | 容器参数按值传递 | 每次调用 O(n) 拷贝 |
| 9 | `check` 里不提前返回 | 求和可能溢出；也白算 |
| 10 | 用 `v.size() - 1` 当初值且 n 可能为 0 | `size_t` 回绕成巨大值 |

## 八、自查

1. 看到 `ld.exe: undefined reference to 'X'`，这句话的准确含义是什么？为什么这次报的是 `WinMain`？
2. 怎么快速判断一个报错是"编译错误"还是"链接错误"？各有什么特征词？
3. `g++ -fsyntax-only 01.cpp` 通过，但链接失败。能由此推出什么结论？
4. 二分答案时，`mid` 为什么必须在循环体内计算？写在外面会发生什么？
5. 为什么"最优锯片高度一定是某棵树的高度"是错的？举个反例。
6. 为什么下标域二分能通过 P1873 的官方样例，却会在评测里 WA？

**答案要点**
1. 有人引用了 X 但没人定义 X。本次是启动代码引用了 `WinMain` 而源文件没有入口函数——实际含义是"没找到 `main`"。MinGW 找 `main` 失败后转而要 `WinMain`，所以报这个名字。
2. 编译错误来自 `cc1plus.exe`，措辞是 `error: ...` 并带 `文件:行:列`；链接错误来自 **`ld.exe`**，措辞是 **`undefined reference to ...`**，结尾有 **`collect2.exe: error: ld returned 1 exit status`**。
3. 说明语法和语义都没问题，问题只可能在"符号定义"——去 `nm` 目标文件看有没有 `main`。
4. 因为 `mid` 是循环的推进变量。写在外面就永远不变，而 `l`/`r` 又被赋成关于 `mid` 的固定值 → 状态不变 → 死循环。
5. 因为 W(H) 在两个相邻树高之间是**线性递减**的，最大的可行整数 H 可以是区间的内部点。反例：`a={20,15}, M=7` → W(14)=7 ✔、W(15)=5 ✘，答案是 14，而 14 不在数组里。
6. 因为那两组样例的答案（15、36）恰好是数组里的元素，下标域二分"蒙对"了；实测随机数据中 95% 的答案不在数组里，一碰上就 WA。

---

## 附录：实测输出

**诊断命令**

```
$ g++ -fsyntax-only 01.cpp
（无输出，退出码 0）                    ← 语法语义完全正常

$ g++ -c 01.cpp -o _probe.o             ← 目标文件生成成功
$ nm _probe.o | grep ' T '
0000000000000000 T _Z1pxSt6vectorIxSaIxEExx
0000000000000113 T _Z4findxSt6vectorIxSaIxEE
（没有 main）
```

**`test_\01_bsanswer_verify.cpp`**

```
===== [1] 反例：答案根本不在数组里 =====
a = {20, 15}, M = 7
  H=17 → W(H)= 3  < M  ✘
  H=16 → W(H)= 4  < M  ✘
  H=15 → W(H)= 5  < M  ✘
  H=14 → W(H)= 7  >= M ✔
  → 最大可行 H = 14，而 14 不在 {20,15} 中
  值域二分得 14（✔）   下标域二分得 0（✘）

===== [2] 随机数据统计（每棵树高 <= 40，M 随机）=====
30000 组随机数据：
  值域二分  错 0 组
  下标域二分 错 16375 组
  真答案【不在】树高数组里的情况：28502 组（占 95.0%）

===== [3] P1873 砍树 官方样例回归 =====
输入 4 7 / 20 15 10 17 → 期望 15，实得 15
  （这组样例的答案 15 恰好在数组里，所以下标域二分也蒙对了：15）
  ⚠️ 这正是最危险的地方：样例过了，交上去照样 WA
```

**`test_\01_bsanswer_fixed.cpp` 回归**

```
4 7  / 20 15 10 17  → 15   （期望 15 ✔）
5 20 / 4 42 40 26 46 → 36   （期望 36 ✔）
2 7  / 20 15        → 14   （期望 14 ✔，且 14 不在数组里）
```
