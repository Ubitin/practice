# 字符串两兄弟：char[] 与 string 完全对照

> 整合《char数组详解.md》与《string的substr详解.md》，把 C 字符串和 C++ string 逐项对照，回答"它们到底差在哪、什么时候用谁"。
> 所有结论与习题答案均已用 g++（C++17）实测（验证程序 `test_\08.cpp`、`test_\quiz3_check.cpp`）。

---

## 0. 先记一张总表（其余都是它的展开）

| 对比项 | `char[]`（C 字符串） | `string`（C++） |
|---|---|---|
| 本质 | 字符数组 + `'\0'` 结尾 | 类对象，**自己记长度** |
| 结尾 `'\0'` | **必须有**（定义属性） | **有**（C++11 起标准保证），但只是"赠品" |
| 长度 | `strlen`（O(n)，数到 '\0'） | `s.size()`（O(1)，直接取） |
| 拼接 | `strcat`（危险，不查空间） | `s += t` / `s + t`（安全） |
| 比较 | `strcmp`（返回 <0/=0/>0） | `==`、`<`（运算符，直观） |
| 修改 | 下标 `s[i]` | 下标 `s[i]` / `at(i)` |
| 输入 | `scanf("%s")`、`fgets` | `cin >> s`、`getline(cin, s)` |
| 传给 C 函数 | 直接传 | `s.c_str()` |
| 内存 | 手动管理（定长/溢出风险） | 自动管理（可扩容） |
| 内部能含 `'\0'` | 不能（遇到即结束） | **能**（二进制数据） |

## 1. 核心区别：'\0' 是"定义"还是"赠品"

**char[] 的 '\0' 是定义**：C 字符串的长度信息只存在 '\0' 里，没有它 strlen 就崩（见《char数组详解》的 '\0' 精确规则）。

**string 的 '\0' 是赠品**：C++11 起标准要求 `s.c_str()`/`s.data()` 返回以 '\0' 结尾的数组——纯粹为了兼容 C 函数（`printf("%s", s.c_str())`、`fopen(s.c_str(),"r")`）。string 认长度靠的是 size()，不是 '\0'。

```cpp
string bin("a\0b", 3);     // 内部藏了一个 '\0'
bin.size();                // 3（'\0' 也算一个字符！）
printf("%s", bin.c_str()); // 输出 "a" —— C 函数在内部 '\0' 处被"骗"截断
cout << bin;               // 完整输出 3 个字符
```

> 一句话：**C 里 `s[i]=='\0'` 就是到头了；C++ 里永远用 `i < s.size()`，别等 '\0'。**

## 2. 逐项对照（每个都配代码）

### 2.1 声明与初始化
```c
char cs[] = "hello";              // 6 格（含 '\0'）
char cs2[10] = "hi";              // 剩余自动补 '\0'
string s = "hello";               // 大小自动，可随时变
```

### 2.2 长度（sizeof / strlen / size 三兄弟）
```c
char cs[] = "hello";
sizeof(cs);    // 6 —— 格子总数（含 '\0'）
strlen(cs);    // 5 —— 字符数（O(n) 扫描）
```
```cpp
string s = "hello";
s.size();      // 5 —— O(1) 直接取，字符串类方法
```
> 课程高频题：`sizeof` vs `strlen`（char 数组）；竞赛习惯：string 一律用 `size()`。

### 2.3 拼接与比较
```c
char buf[32] = "hello";
strcat(buf, " world");            // 不检查空间，溢出风险
strcmp(buf, "hello")              // 比较：<0/=0/>0，不能写 ==
```
```cpp
string s = "hello";
s += " world";                    // 自动扩容，安全
s == "hello"                      // 直观；s < t 还支持字典序比较
```

### 2.4 输入
```c
scanf("%s", cs);        // 读一个词，遇空白停
fgets(cs, 32, stdin);   // 读整行（保留 '\n'）
```
```cpp
cin >> s;               // 读一个词
getline(cin, s);        // 读整行（不含 '\n'，比 fgets 省心）
```

### 2.5 互转（考试/项目高频）
```cpp
string s = "hello";
const char* p = s.c_str();        // string → C 字符串（只读）
string t = p;                     // C 字符串 → string（自动构造）
string u("hello", 3);             // 指定长度构造（能包含内部 '\0'）
```

### 2.6 一个必须知道的坑：c_str() 指针可能失效
```cpp
string s = "hello";
const char* p = s.c_str();        // 拿到内部缓冲区的指针
s[0] = 'H';                       // 修改 string（可能触发扩容/移动）
printf("%s", p);                  // ⚠️ p 可能已失效（实测小字符串恰好没搬家，但绝不能依赖）
```
> 和迭代器失效同一个道理：**string 被修改/扩容后，之前取得的 c_str() 指针可能悬垂**——要用就现取，别存着跨修改使用。

## 3. 什么时候用谁（三视角）

| 场景 | 用谁 | 原因 |
|---|---|---|
| [竞赛] 字符串模拟题（P1553、P1308） | `string` | 拼接/比较/取子串都省心，`size()` O(1) |
| [竞赛] 快读/大数据 | `scanf("%s")` + char 数组 | 比 cin 快一个常数 |
| [课程] C 语言课/期末 | `char[]`（按教材） | 考点就是 strlen/strcmp/'\\0' |
| [课程] 课设（学生管理） | 两者皆可 | 姓名存储用 char[] 或 string 都行，看教材 |
| [项目] 与 C 库交互（printf/fopen/网络） | `string` + `c_str()` | string 管内存，c_str() 负责"翻译" |
| [项目] 二进制数据/协议解析 | `string`（可含内部 '\0'） | char[] 一遇 '\0' 就废 |

## 4. 常见坑对照表

| 坑 | char[] 版 | string 版 |
|---|---|---|
| 长度 | sizeof/strlen 混用、传参退化 | `size()` 与 `length()` 相同，无此坑 |
| 溢出 | strcpy/strcat 不查空间 | 自动扩容，无此坑 |
| 比较 | 写 `==` 比较的是地址 | 直接 `==` 就是内容比较 |
| 结尾 | 忘 '\0' → strlen 越界 | 末尾 '\0' 由标准保证 |
| 内部 '\0' | 不可能 | 可能，C 函数会截断 |
| 指针 | 野指针/悬垂 | c_str() 在修改后可能失效 |

## 5. 习题（答案已实测，先做再看）

**E1** 写出输出：
```cpp
char cs[] = "abc";
string ss = "abc";
printf("%zu %zu %zu %d\n", sizeof(cs), strlen(cs), ss.size(), (int)ss[3]);
```

**E2** 纠错：下面代码编译报错，为什么？怎么改？
```cpp
string s = "hello";
printf("%s\n", s);        // ❌
```

**E3**（多选）判断对错：
```cpp
A. string 的 size() 一定等于 strlen(s.c_str())
B. string 的末尾一定有一个 '\0'
C. char s[10] = "hi" 的 strlen 是 2
D. string 里不能包含字符 '\0'
```

**E4** 写出输出：
```cpp
string a = "abc", b = "abd";
cout << (a < b) << " " << (a == "abc") << "\n";
```

**E5** 写出输出（输入为 `hello world`）：
```cpp
char buf[20];
scanf("%s", buf);
printf("%zu\n", strlen(buf));
```

**E6** 概念题：`const char* p = s.c_str();` 之后对 `s` 做修改（如 `s[0]='H'` 或 `s += "x"`），`p` 还可靠吗？为什么？

---

## 答案与解析

**E1** → `4 3 3 0`
sizeof=4（abc+\0）；strlen=3；ss.size()=3；ss[3] 是 '\0'（下标等于 size() 合法）。

**E2** → `printf` 的 `%s` 需要 C 字符串（const char*），string 对象不能直接传。改：
```cpp
printf("%s\n", s.c_str());   // 或干脆 cout << s;
```

**E3** → **B、C 对；A、D 错**。
A 错：string 内部可以含 '\0'（此时 strlen 小于 size()）；B 对：C++11 起末尾保证 '\0'；C 对：列表初始化剩余补 0；D 错：string 可以存二进制数据。

**E4** → `1 1`
`a < b` 按字典序为真（1）；`a == "abc"` 内容相等为真（1）——这就是 string 相比 strcmp 的直观优势。

**E5** → `5`
`scanf("%s")` 遇空格停，只读到 "hello"，strlen=5。

**E6** → **不一定可靠**。`c_str()` 返回指向 string 内部缓冲区的指针；修改 s 可能触发扩容/内存搬移，旧指针悬垂（与迭代器失效同理）。**要用就现取**：每次传参时写 `s.c_str()`，别存起来跨修改使用。（实测小字符串不扩容时恰好仍可用，但这是未定义行为，绝不能依赖。）

---

**一句话总结**：**char[] 是"手动挡"（'\0' 定义、自己管内存、C 函数原生），string 是"自动挡"（size() 认长度、自动扩容、运算符直观，但'\0' 只是兼容 C 的赠品）**——竞赛写题用 string，C 课程按教材用 char[]，项目里两者用 `c_str()` 桥接。
