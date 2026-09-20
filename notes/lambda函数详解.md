# lambda 函数详解 —— 语法、捕获、实战比较器与坑

> 定位：`lambda` 在你的讲义里已经被用了很多次（sort 比较器、`count_if`、`check` 函数……），但从没系统讲过。
> 一句话：**lambda 就是一个"就地写出来的匿名函数对象"**，语法是 `[捕获列表](参数列表) -> 返回类型 { 函数体 }`，能省的都能省。
> 实测程序：`test_\36.cpp`（主）、`test_\36_strictweak.cpp`（比较器违反严格弱序的后果）、`test_\36_staticcap.cpp`（静态变量捕获语义）。
> 相关：`机试代码模板速查手册` §3.1（排序）、§3.2（二分）、§3.12（前缀最小值）。

---

## 一、为什么需要 lambda

以前想给 `sort` 传自定义规则，得在**外面**写一个函数：

```cpp
bool cmpDesc(int a, int b) { return a > b; }     // 离使用处很远
sort(v.begin(), v.end(), cmpDesc);
```

问题：
- 函数写在别处，读代码要来回跳
- 名字会污染全局作用域（重名风险）
- 想用局部变量当参数，还得靠全局变量传进去

lambda 解决全部三点：**就地写、不用起名、能直接捕获外面的变量。**

```cpp
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
```

## 二、基本语法

```
[捕获列表] (参数列表) -> 返回类型 { 函数体 }
    ↑           ↑          ↑          ↑
  必须有     可省()     可省        必须有
```

**实测**：

```cpp
auto f1 = []() { return 42; };                  // f1() = 42
auto f2 = [](int a, int b) { return a + b; };   // f2(3,4) = 7
auto f3 = [](double x) { return x * 2; };       // f3(1.5) = 3
auto f4 = [](int x) -> ll { return x * 100000000LL; };  // 显式指定返回类型
auto f5 = [](int x) { return x + 1; };          // f5(1) = 2
```

**能省什么**：
- 没有参数 → 可以省掉 `()`
- 返回类型能推导 → 省掉 `-> T`
- 只有 `return 表达式;` 一条语句 → 可以省掉 `return` 和 `{}`

**返回值一定要接住**：lambda 的类型是**匿名的**，写不出来，所以只能用 `auto`：

```cpp
auto f = [](int x) { return x; };     // ✅
// ??? f = [](int x) { return x; };   // ❌ 没有类型名可写
```

## 三、⭐ 捕获列表全景（7 种写法）

| 写法 | 含义 |
|---|---|
| `[]` | 不捕获任何外部变量 |
| `[=]` | 全部**按值**捕获（拷副本进闭包） |
| `[&]` | 全部**按引用**捕获（存指针，改得到原变量） |
| `[a]` | 只按值捕获 `a` |
| `[&a]` | 只按引用捕获 `a` |
| `[=, &a]` | 默认按值，但 `a` 用引用 |
| `[&, a]` | 默认按引用，但 `a` 用值 |

**实测输出**（`a=1, b=2, c=3`）：

```
[]        = 0
[=]       = 6
[&]       = 6
[a]       = 1
[&a]      = 1
[=, &a]   = 6
[&, a]    = 6
```

**两条语法规则**：
1. 混合写法里，`=` 或 `&` **只能出现在第一个位置**，后面的必须是具名变量
2. 标准不允许重复：`[=, a]`、`[&, &a]` 都是非法的
   > ⚠️ 实测：**GCC 默认模式会宽容接受**这两个写法（只报错于 `-pedantic-errors`）：
   > `error: explicit by-copy capture of 'a' redundant with by-copy capture default`
   > **换个编译器就可能编译不过**——别写。

## 四、⭐⭐ 值捕获 = 定义那一刻的【快照】

**这是最容易搞错的一条。**

```cpp
int x = 10;
auto byValue = [x]() { return x; };     // 把 x 拷一份进闭包
auto byRef   = [&x]() { return x; };    // 引用外面那个 x
x = 20;
```

**实测**：

```
定义时 x=10，之后改成 x=20
[x]  值捕获读到的 = 10   ← 还是 10（定义时的快照）
[&x] 引用捕获读到的 = 20   ← 跟着变成 20
再把 x 改成 30：[x] = 10，[&x] = 30
```

**结论**：值捕获是把值**拷贝进闭包对象**，之后再改原变量与它无关。引用捕获存的是"指向原变量的指针"，原变量一变它就看到新值。

> 记忆法：**值捕获拍照片，引用捕获开直播。**

## 五、mutable：让副本可改，且状态跨调用保留

值捕获的副本默认是**只读**的。想改它要加 `mutable`：

```cpp
int cnt = 0;
auto inc = [cnt]() mutable { return ++cnt; };
```

**实测**：

```
依次调用：1 2 3
外面的 cnt 还是 0  ← mutable 改的是副本，不是原变量
```

**注意两点**：
1. `mutable` 改的是**闭包内的副本**，外面的 `cnt` 不变（想改外面的必须用 `[&cnt]`，实测调用一次后 `cnt = 1`）
2. **状态跨调用保留**——这就是"闭包/有状态函数"的用法

### ⚠️ 有状态 lambda 的一个坑：不能在同一个表达式里多次调用

```cpp
// ❌ 实参求值顺序【未指定】
printf("%d %d %d", inc(), inc(), inc());
```

本机实测这种写法输出的是 **`3 2 1`**（从右往左求值），而不是 `1 2 3`。**分成三条语句写就确定有序**。

> 函数实参的求值顺序在 C++ 里是未指定的，涉及有副作用的表达式时顺序完全依赖实现。

## 六、⚠️ 全局 / 静态局部变量：不需要捕获，「捕获」它们反而是陷阱

```cpp
int gv = 100;                      // 全局变量
auto f = []() { return gv; };      // ✅ 无需捕获，直接读

void foo() {
    static int sv = 7;             // 静态局部变量
    auto g = []() { return sv; };  // ✅ 同样无需捕获
}
```

**原因**：捕获只能捕获**自动存储期**（automatic storage duration）的变量。全局变量和 `static` 变量的生命周期本来就是整个程序，直接访问即可。

### ⭐ 陷阱：对 `static` 写"值捕获"，快照语义不成立

**实测**（`test_\36_staticcap.cpp`）：

```cpp
static int sv = 7;
auto capVal = [sv]() { return sv; };    // 写了 [sv]，以为是快照
auto noCap  = []()   { return sv; };    // 不写捕获
sv = 99;
```

```
[sv]() 读到 = 99    noCap() 读到 = 99
```

**两个都是 99！** 说明 `[sv]` 这个"值捕获"**根本没有拷副本**——静态变量永远是直接访问。

```
warning: capture of variable 'sv' with non-automatic storage duration
```

**GCC 默认只给警告**（加 `-pedantic-errors` 才变成 error）。于是你以为拿到了快照，实际拿到的是"直播"——**和 `[&sv]` 行为一样**。

> 结论：**别对全局变量和 static 变量写捕获列表**，直接不写就行。

## 七、返回类型推导与它的坑

**规则**：如果 `return` 语句能推出唯一类型，就自动推导；否则**编译错误**（不是默默选一个）。

```cpp
// ❌ 编译错误
auto f = [](int x) { if (x) return 1; else return 2.5; };
```

**实测报错原文**：

```
error: inconsistent types 'int' and 'double' deduced for lambda return type
```

**修法**：显式写返回类型，或者让所有 `return` 类型一致。

```cpp
auto f = [](int x) -> double { if (x) return 1; else return 2.5; };   // ✅
```

> 这一点和普通函数不同（普通函数会做隐式转换），lambda 的推导**不做类型统一**，直接报错。好处是不会静默丢精度。

## 八、泛型 lambda（C++14）：参数写 `auto`

```cpp
auto add = [](auto x, auto y) { return x + y; };
```

**实测**：

```
add(3, 4)          = 7
add(1.5, 2.25)     = 3.75
add(string,string) = lambda
```

**本质**：编译器为每种实参类型各生成一个 `operator()`，就是一个模板化的闭包。

⚠️ **C++11 不支持**：

```
error: use of 'auto' in lambda parameter declaration only available with '-std=c++14' or '-std=gnu++14'
```

## 九、递归 lambda：三种写法

lambda 在自己函数体里**看不到自己**（名字还没定义完），所以不能直接递归。

### ① 把自己当参数传进去（C++14，推荐）

```cpp
auto fib = [](auto&& self, int n) -> ll {
    return n < 2 ? n : self(self, n - 1) + self(self, n - 2);
};
fib(fib, 30);
```

⚠️ 必须写 `self(self, ...)`，只写 `self(...)` 编译不过。

### ② `std::function`（写法直观，但慢）

```cpp
function<ll(int)> fib2 = [&](int n) -> ll {
    return n < 2 ? n : fib2(n - 1) + fib2(n - 2);
};
```

### ③ C++23 `(this auto&& self)`

```cpp
auto fib3 = [](this auto&& self, int n) -> ll { ... };   // 需要 -std=c++23
```

### **实测性能对比**（fib(30)）

| 写法 | 耗时 |
|---|---|
| ① 自传参 `(auto&& self, ...)` | **0.778 ms** |
| ② `std::function<ll(int)>` | **3.119 ms** |
| 比值 | **4.0 倍** |

> `std::function` 有**类型擦除**开销：每次调用都要走一次间接跳转（类似虚函数），而且**无法内联**。

## 十、⭐ 实战：三种比较器（含最容易搞反的方向）

### 10.1 `sort` 的比较器

```cpp
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });   // 降序
```

**`cmp(a, b)` 返回 true 表示 a 应该排在 b 前面。**

### 10.2 `priority_queue` 的比较器 —— **必须作为构造参数传入**

```cpp
auto cmp = [](int a, int b) { return a > b; };
priority_queue<int, vector<int>, decltype(cmp)> pq(cmp, vector<int>(v.begin(), v.end()));
```

**实测**（同一个 `cmp`，输入 `{3,1,4,1,5,9,2,6}`）：

```
sort(v, cmp) 结果：9 6 5 4 3 2 1 1     ← 降序
priority_queue 的 top() = 1            ← 堆顶是【最小值】（小根堆）
```

> **⚠️ 方向感口诀**：`cmp(a,b) == true` 表示 a 排在 b **前面**。
> `sort` 输出"从前往后"；`priority_queue` 的 `top()` 是"**最前面那个**"。
> 所以 `[](int a,int b){return a>b;}` 对 sort 是降序、对堆是小根堆——**同一个 lambda，效果相反**。

⚠️ 只写类型不传对象是编译不过的，`decltype(cmp)` 拿类型、`cmp` 拿值，**两个都要**。

### 10.3 `set` / `map` 的比较器

```cpp
auto scmp = [](int a, int b) { return a > b; };
set<int, decltype(scmp)> s(scmp);      // 降序 set
```

**实测**：遍历输出 `9 6 5 4 3 2 1`（降序且自动去重）。

⚠️ lambda 类型是匿名的，必须用 `decltype(...)` 拿到；且要把对象传给构造函数。

## 十一、实战：常见算法里的 lambda

```cpp
count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });          // 条件计数
find_if(v.begin(), v.end(),  [](int x) { return x > 4; });               // 找第一个满足的
remove_if(t.begin(), t.end(),[](int x) { return x < 3; });               // 配合 erase 真删
accumulate(v.begin(), v.end(), 1, [](int acc, int x) { return acc * x; });// 自定义归约
nth_element(v.begin(), v.begin()+2, v.end(), [](int a,int b){return a>b;});// 第 k 大
```

**实测**：

```
count_if 偶数个数        = 3
find_if 第一个 >4 的位置 = 下标 4（值 5）
remove_if 删掉 <3 后：3 4 5 9 6
accumulate 求和          = 31
accumulate 求积(带 lambda) = 6480
nth_element 按降序取第 3 大 = 5
```

## 十二、⚠️ 比较器必须满足【严格弱序】，否则崩溃

`std::sort` 要求比较器满足 **strict weak ordering**，最核心的两条：

- `comp(a, a)` 必须是 **false**（不可自反）
- `comp(a, b)` 与 `comp(b, a)` **不能同时为 true**

**写成 `<=` 或 `>=` 就违反了这两条 → 未定义行为。**

### 实测后果（`test_\36_strictweak.cpp`，N 个数排序）

| 比较器 | 值域（重复程度） | 结果 |
|---|---|---|
| `a < b`（正确） | 1e6（几乎不重复） | 正常 |
| **`a <= b`（错误）** | 1e6（几乎不重复） | **侥幸正常** |
| **`a <= b`（错误）** | 5（大量重复），N=20 万 | **崩溃 0xC0000374（堆损坏）** |
| **`a <= b`（错误）** | 3（大量重复），N=50 万 | **崩溃 0xC0000005（访问违规）** |
| **`a <= b`（错误）** | 2（只有 0/1），N=100 万 | **崩溃 0xC0000005** |
| **`a <= b`（错误）** | 2，N=200 万 | **崩溃 0xC0000005** |

> **两条血泪教训**：
> 1. **重复值越多，破坏越严重。** 值几乎不重复时 `<=` 和 `<` 行为一致，bug 完全隐藏——**这正是它容易漏掉的原因**。
> 2. 崩溃类型是**堆损坏或越界访问**（`sort` 内部走错分支导致数组越界读写），报错位置和真正的 bug 毫无关系，极难排查。

**正确写法**：

```cpp
sort(v.begin(), v.end(), [](int a, int b) { return a < b; });    // ✅
sort(v.begin(), v.end(), [](int a, int b) { return a > b; });    // ✅ 降序
sort(v.begin(), v.end(), [](int a, int b) { return a <= b; });   // ❌ UB
```

**结构体多关键字**同理，要写成"先比第一关键字，相等时严格比第二关键字"：

```cpp
sort(v.begin(), v.end(), [](const Stu& x, const Stu& y) {
    if (x.score != y.score) return x.score > y.score;   // ✅ 不相等才比较
    return x.id < y.id;
});
```

## 十三、性能：lambda vs `std::function`；捕获方式

### 13.1 别用 `std::function` 做比较器

**实测**（排序 200 万个随机数）：

| 比较器 | 耗时 |
|---|---|
| lambda（无捕获，可直接内联） | **97.8 ms** |
| `std::function<bool(int,int)>` | **179.2 ms** |
| 比值 | **1.83 倍** |

> `std::function` 是**类型擦除**的容器，调用要走间接跳转、无法内联。排序要做约 `n log n` 次比较，这个开销被放大几千万倍。
> **机试里除了递归 lambda，一律别用 `std::function`。**

### 13.2 捕获方式：值捕获大对象的代价在"构造那一次"

**实测**（100 万元素 = 4 MB 的 vector，在循环里各构造 lambda 200 次）：

| 捕获方式 | 耗时 |
|---|---|
| `[big]` 值捕获 | **105.782 ms**（每次构造真拷一份，共 0.80 GB） |
| `[&big]` 引用捕获 | 测不出（只存一个指针） |

**结论**：
- 传给 `sort` 的比较器**只构造一次**，值捕获无所谓，随便写
- 危险的是**在循环内反复构造值捕获大对象的 lambda**——每次都是一次完整拷贝
- 想在 lambda 内**修改**外部变量，也必须用引用捕获

## 十四、版本差异速查

| 特性 | 起始标准 | 写法 |
|---|---|---|
| lambda 基本语法 | C++11 | `[](){}` |
| 泛型 lambda（`auto` 参数） | C++14 | `[](auto x){}` |
| 初始化捕获（把表达式搬进闭包） | C++14 | `[p = make_unique<T>()](){}` |
| `constexpr` lambda | C++17 | `[]() constexpr {}` |
| 捕获 `*this`（拷贝整个对象） | C++17 | `[*this](){}` |
| 模板 lambda | C++20 | `[]<class T>(T x){}` |
| 捕获结构化绑定 | C++20 | `auto [a,b]=...; [a,b](){}` |
| `(this auto&& self)` 自递归 | C++23 | `[](this auto&& self, int n){}` |

> 机试环境一般是 C++14/17，**能用的是前 5 行**，后面的别用。

## 十五、坑清单

| # | 坑 | 说明 |
|---|---|---|
| 1 | 以为值捕获"跟着变" | 值捕获是**定义时的快照**；要跟变得用 `[&x]` |
| 2 | 以为 `[&]` 事事方便 | 引用捕获有**悬垂风险**：lambda 活得比变量久就 UB（比如函数返回后还被调用） |
| 3 | 改值捕获的副本没加 `mutable` | `error: increment of read-only variable 'c'` |
| 4 | 以为 `mutable` 改得到外面的变量 | 改的是副本，外面不变 |
| 5 | 一个表达式里多次调用有状态 lambda | 实参**求值顺序未指定**，实测输出 `3 2 1` |
| 6 | **对 static / 全局变量写捕获** | GCC 只给警告，且**捕获被忽略**（实测 `[sv]` 读到 99 而非 7） |
| 7 | 多个 `return` 类型不一致 | `error: inconsistent types ... deduced for lambda return type`；lambda 不做隐式统一 |
| 8 | **比较器写 `<=` / `>=`** | 违反严格弱序 → **UB，实测在重复值多时崩溃**（堆损坏/越界） |
| 9 | **`priority_queue` 的比较器方向搞反** | 同一个 lambda，sort 降序、堆是小根堆；口诀：true = a 排前面 |
| 10 | `priority_queue` 只写类型不传对象 | 必须 `pq(cmp)`，`decltype` 拿类型、`cmp` 拿值 |
| 11 | 用 `std::function` 做比较器 | 实测慢 1.83 倍，且无法内联 |
| 12 | 循环里反复构造值捕获大对象的 lambda | 每次都是一次完整拷贝（实测 200 次 4MB = 105 ms） |
| 13 | C++11 里写泛型 lambda | 需要 C++14 |
| 14 | `[=, a]` / `[&, &a]` | 标准非法；GCC 宽容但换编译器可能报错 |
| 15 | 递归 lambda 写 `self(...)` | 必须写 `self(self, ...)` |
| 16 | 捕获列表写 `[x, x]` 或捕获不存在的名字 | 编译错误，且不会自动帮你找 |

## 十六、自查

1. lambda 的完整语法是什么？哪三处可以省略？
2. `[x]` 和 `[&x]` 的区别是什么？如果定义 lambda 之后修改 `x`，两者分别读到什么？
3. 想在一个 `[cnt]` 值捕获的 lambda 里改 `cnt`，要加什么关键字？改的是谁？
4. 对 `static` 局部变量写 `[sv]`，真的会拷贝一份快照吗？实测结论是什么？
5. `sort` 和 `priority_queue` 传同一个 `[](int a,int b){return a>b;}`，结果分别是什么？为什么？
6. 比较器写成 `<=` 会发生什么？为什么"值几乎不重复"时不容易发现？
7. `std::function` 做比较器为什么慢？慢多少？
8. 递归 lambda 的两种主流写法是什么？哪种快，快多少？

**答案要点**
1. `[捕获列表](参数列表) -> 返回类型 { 函数体 }`。可以省：无参数时的 `()`、能推导时的 `-> T`、单表达式时的 `return` 和 `{}`。
2. `[x]` 把值拷进闭包，`[&x]` 存指向原变量的引用。之后改 `x`：`[x]` 读到的还是定义时的旧值（实测 10），`[&x]` 读到新值（实测 20）。
3. 加 `mutable`。改的是**闭包内的副本**，外面的 `cnt` 不变（实测外面仍是 0）；想改外面必须用 `[&cnt]`。
4. 不会。实测 `[sv]` 与不捕获的 `[]` 都读到 99，说明"值捕获"被忽略、始终直接访问静态变量。GCC 默认只给 `warning: capture of variable 'sv' with non-automatic storage duration`，`-pedantic-errors` 才是 error。
5. `sort` 得到降序（`9 6 5 4 3 2 1 1`），`priority_queue` 的 `top()` 是最小值（小根堆）。因为 `cmp(a,b)==true` 的含义是"a 排在 b 前面"：sort 从前往后输出，堆顶是"最前面那个"。
6. 违反严格弱序（`comp(a,a)` 为 true）→ 未定义行为。实测在重复值多时**直接崩溃**（N=20 万/值域 5 时 0xC0000374 堆损坏；值域 2/3 时 0xC0000005 访问违规）。值几乎不重复时 `<=` 与 `<` 结果一致，bug 完全隐藏，所以极难发现。
7. `std::function` 有类型擦除，调用走间接跳转且无法内联。实测排序 200 万个数慢 **1.83 倍**（179.2ms vs 97.8ms）。
8. ①`(auto&& self, ...)` 自传参，需要写 `self(self, ...)`；②`std::function`。实测 fib(30)：**0.778ms vs 3.119ms，快 4.0 倍**。

---

## 附录：实测输出

**`test_\36.cpp`（节选）**

```
========== [2] ⭐ 值捕获 = 定义那一刻的【快照】 ==========
  定义时 x=10，之后改成 x=20
  [x]  值捕获读到的 = 10   ← 还是 10（定义时的快照）
  [&x] 引用捕获读到的 = 20   ← 跟着变成 20
  再把 x 改成 30：[x] = 10，[&x] = 30

========== [4] mutable ==========
  [cnt]() mutable { return ++cnt; } 依次调用：1 2 3
  外面的 cnt 还是 0
  换成 [&cnt]，调一次后外面 cnt = 1

========== [5] 静态变量捕获陷阱 ==========
  static sv 从 7 改成 99 后：
    [sv]() 读到 = 99    noCap() 读到 = 99
  ⚠️ 两个都是 99 —— 说明 [sv] 这个「值捕获」根本没拷副本

========== [7] 递归 lambda ==========
  fib(30) = 832040 / 832040
  ① 自传参 (auto&& self)  :    0.778 ms
  ② std::function<ll(int)> :    3.119 ms
  → 比值 4.0 倍

========== [8] 同一个 lambda 在 sort 与 priority_queue 上方向相反 ==========
  sort(v, cmp) 结果：9 6 5 4 3 2 1 1   ← 降序
  priority_queue 的 top() = 1  ← 堆顶是【最小值】

========== [11] lambda vs std::function（排序 200 万个数）==========
  lambda（无捕获，可直接内联） :     97.8 ms
  std::function               :    179.2 ms
  → 慢 1.83 倍

========== [12] 循环里构造值捕获大对象 ==========
  1000000 元素(4 MB) 的 vector，各构造 lambda 200 次：
  [big]  值捕获  :  105.782 ms   ← 共 0.80 GB 拷贝
  [&big] 引用捕获 :    0.000 ms
```

**`test_\36_strictweak.cpp`**

```
mode=0 (正确的<) N=200000   range=1000000  → sort 返回，结果有序（本次未暴露）
mode=1 (错误的<=) N=200000   range=1000000  → sort 返回，结果有序（本次未暴露）
mode=1 (错误的<=) N=200000   range=5        → ★ 崩溃，退出码 -1073740940 (0xC0000374 堆损坏)
mode=1 (错误的<=) N=500000   range=3        → ★ 崩溃，退出码 -1073741819 (0xC0000005 访问违规)
mode=1 (错误的<=) N=1000000  range=2        → ★ 崩溃，退出码 -1073741819
mode=1 (错误的<=) N=2000000  range=2        → ★ 崩溃，退出码 -1073741819
```

**编译错误实测原文**

```
ret_type:     error: inconsistent types 'int' and 'double' deduced for lambda return type
mutable_no:   error: increment of read-only variable 'c'
cpp11_auto:   error: use of 'auto' in lambda parameter declaration only available with '-std=c++14'
cap_static:   warning: capture of variable 'sv' with non-automatic storage duration   （-pedantic-errors 下为 error）
dup_cap:      error: explicit by-copy capture of 'a' redundant with by-copy capture default      （仅 -pedantic-errors）
dup_cap2:     error: explicit by-reference capture of 'a' redundant with by-reference capture default （仅 -pedantic-errors）
```
