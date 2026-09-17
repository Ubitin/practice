# to_string 函数详解（数字 → 字符串）

> 一句话：`std::to_string(x)` 把**数字** x 变成 `std::string`，C++11 起可用，头文件 `<string>`（`bits/stdc++.h` 已包含）。
> 本文所有输出均为 **g++ 16.2 实测结果**，不是猜的。配套反向操作 `stoi/stoll/stod`（字符串 → 数字）一并讲。

---

## 1. 基本用法：对哪个类型都能用

重载齐全：`int / long / long long / unsigned / unsigned long / unsigned long long / float / double / long double`，统一返回 string。

```cpp
string a = to_string(123);              // "123"
string b = to_string(-12);              // "-12"（负号保留）
string c = to_string(1234567890123LL);  // "1234567890123"（long long 大数）
string d = to_string(0);                // "0"
```

实测输出对照：

| 调用 | 结果 |
|---|---|
| `to_string(123)` | `123` |
| `to_string(-12)` | `-12` |
| `to_string(1234567890123LL)` | `1234567890123` |
| `to_string(0)` | `0` |

> ⚠️ 字面量默认是 `int`：`to_string(1234567890123)` 不带 `LL` 会先溢出再转，结果错——大数记得写 `LL` 后缀。

---

## 2. 什么时候用它

### 2.1 拼字符串 / 输出
```cpp
cout << "总分：" + to_string(score) + " 分\n";   // 直接拼接（有 string 就能 +）
string s = "id" + to_string(id);                 // 生成编号 "id7"、"id42"
```
其实 `cout << 数字` 也行；to_string 的真正价值是**把数字变成字符串本身**，从而用字符串那套工具。

### 2.2 数字也能用字符串算法
```cpp
int x = 20260905;
string s = to_string(x);
int len = (int)s.size();                 // 位数（负数会多算 1 个 '-'）
int sum = 0; for (char ch : s) sum += ch - '0';   // 各位数字之和
reverse(s.begin(), s.end());             // 数字反转（P1553 那类题）
s.find('0'); s.substr(0, 2);             // 任意字符串操作照常可用
```

---

## 3. 大坑专区（全部实测）

### 坑 ①：char 传进去变成 ASCII 码！
`to_string('A')` 结果是 **`"65"`**，不是 `"A"`。
原因：to_string **没有 char 重载**，char 自动提升成 int → 走 to_string(int) → 输出 65。
`to_string('0')` 同理得到 `"48"`。

要"单个字符的字符串"，正确姿势：
```cpp
string s(1, 'A');          // "A"
s.push_back('A');
string t = "A";            // 直接写字符串字面量最省事
```

### 坑 ②：浮点数永远是"6 位小数"（printf %f 规则）
| 调用 | 结果 | 问题 |
|---|---|---|
| `to_string(100.0)` | `100.000000` | 整数也补 6 个 0 |
| `to_string(3.141592653589793)` | `3.141593` | 四舍五入到 6 位 |
| `to_string(1e-7)` | `0.000000` | **精度全丢**（太小） |
| `to_string(1e20)` | `100000000000000000000.000000` | 不用科学计数法、尾随 6 个 0 |

浮点想按自己的格式输出，**别用 to_string**，用格式化三件套：
```cpp
printf("%.2f\n", 3.14159);                       // C 风格
ostringstream os; os << fixed << setprecision(2) << 3.14159;   // C++ 风格（实测 → "3.14"）
```

### 坑 ③：bool 变成 "1"/"0"
`to_string(true)` → `"1"`（bool 提升成 int）。要 "true"/"false" 自己写三目：
```cpp
string s = ok ? "true" : "false";
```

### 坑 ④：位数统计别忘负号
`to_string(-7).size()` 是 **2**（"-7"），只有正数才等于位数。

---

## 4. 反向操作：stoi / stoll / stod（string → 数字）

| 函数 | 作用 | 实测行为 |
|---|---|---|
| `stoi(s)` | → int | 从头吃到第一个非法字符为止：`stoi("42abc")` = 42 |
| `stoll(s)` | → long long | 大数用它，别用 stoi（int 溢出） |
| `stod(s)` | → double | `stod("3.5xyz")` = 3.5 |

两个会**抛异常**的情形（不想被炸就 try-catch 或保证输入合法）：
- 一个数字都吃不到：`stoi("abc")` → 抛 `invalid_argument`
- 数值超出类型范围：`stoi("99999999999999999999999")` → 抛 `out_of_range`

> 对拍/题目里读入数字几乎都用 `cin >>`，stoi 族主要用于"已经拿到字符串、要转数字"（如分割字段后转换）。

---

## 5. 速记卡

```
to_string(x)  → 数字变字符串（int/LL/unsigned/double 全家桶）
  ├ 负数保留 '-'，正数没 '+'（"+5" 不是 to_string 的输出）
  ├ char/bool 会提升成 int → 'A'→"65"、true→"1"（没有对应重载！）
  └ double 恒 6 位小数 → 要精度用 printf / ostringstream << fixed << setprecision

stoi / stoll / stod（s） → 字符串变数字，能吃前段、全非法或越界会抛异常
```

---

## 6. 自测（3 分钟）

1. `to_string(-2026).size()` 等于几？
2. 想得到 `"A"`（字符串），`to_string('A')` 行吗？正确写法？
3. `to_string(1.0 / 8)` 输出什么？（提示：小数恒 6 位）
4. `stoi("3.14")` 会怎样？`stoi("314px")` 呢？

答案：1. 5（"-2026" 含负号）。2. 不行，得到 "65"；用 `string(1,'A')` 或 `"A"`。3. `0.125000`。4. `stoi("3.14")` = 3（吃到 '.' 停）；`stoi("314px")` = 314。
