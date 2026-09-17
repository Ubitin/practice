# string 的 substr() 详解

> 前置：STL 讲义 §4（string 基础）、洛谷 P1553 思路（substr 就是拆数的核心工具）。
> 演示程序：`test_\06.cpp`（全部示例已用 g++ 跑通）。

---

## 1. 一句话本质 + 函数签名

**substr = 从字符串中"切出一段"生成新字符串**（`substring` 的缩写）。

```cpp
string substr(size_t pos = 0, size_t count = npos) const;
// pos   : 从哪个下标开始（默认 0 = 开头）
// count : 切多长（默认 npos = "一直切到结尾"）
```

两种用法对应两个常见需求：

```cpp
string s = "Hello World";      // 下标：H0 e1 l2 l3 o4 ' '5 W6 o7 r8 l9 d10
s.substr(6);        // "World"     —— 只要起点：从 6 切到结尾
s.substr(0, 5);     // "Hello"     —— 起点+长度：从 0 开始切 5 个
```

## 2. 用"区间"理解（左闭右开，呼应你学的迭代器）

`substr(pos, count)` 切的是下标区间 **`[pos, pos + count)`**——**包含 pos，不包含 pos+count**：

```
Hello World
0123456789 10
  ^^^
  substr(1, 3) = "ell"    // 下标 1,2,3（3 = 1+3-1 是最后一个；4 = 1+3 不包含）
```

> 记忆钩子：**和 `begin()/end()` 一样是左闭右开**——`substr(pos, count)` 里的 count 是"长度"不是"结束下标"，别写成 `substr(pos, pos+count)`。

## 3. 边界行为（必背表，笔试常考）

| 情况 | 行为 | 示例（s = "Hello World"，size=11） |
|---|---|---|
| `pos == size()` | 合法，返回**空串** | `s.substr(11)` → `""` |
| `pos > size()` | **抛 `out_of_range` 异常**（崩溃） | `s.substr(100)` → 抛异常 |
| `count` 超过结尾 | **自动截到结尾**（不报错） | `s.substr(3, 100)` → `"lo World"` |
| 省略 count | 等价 `count = npos`，切到结尾 | `s.substr(6)` ≡ `s.substr(6, npos)` |
| 空串 | 正常返回空串 | `"".substr(0)` → `""` |

## 4. 三个高频应用场景

**① 取文件名后缀 / 扩展名**
```cpp
string file = "photo.jpg";
size_t dot = file.find('.');
string ext = file.substr(dot + 1);    // "jpg"（dot 后面开始，正好跳过 '.'）
```

**② 按分隔符分割（P1553 拆数就是这套！）**
```cpp
string num = "600.084";
size_t p = num.find('.');
string a = num.substr(0, p);      // "600" —— '.' 之前
string b = num.substr(p + 1);     // "084" —— '.' 之后
```
> 万能公式：**分割 = `find(分隔符)` 找到位置 p + `substr(0,p)` 取左边 + `substr(p+1)` 取右边**。P1553 的整数/小数、分数/分母、P1308 统计单词数全是这个套路。

**③ 取定长字段（学号 / 身份证 / 日期）**
```cpp
string id = "2024090101";
id.substr(0, 4);   // "2024"（年）
id.substr(4, 2);   // "09" （月）
```

## 5. 复杂度与拷贝代价（竞赛必知）

- `substr` **返回一个新字符串（拷贝）**，复杂度 **O(count)**。
- 小字符串无所谓；但**在循环里对大字符串反复 substr = 反复拷贝 = 超时风险**（n=1e5 级别的字符串题尤其）。
- 只读查看、不修改时，可以用 **`string_view`（C++17）** 零拷贝：
```cpp
string_view sv = s;                 // 不拷贝
string_view part = sv.substr(6, 5); // string_view 也有 substr，O(1) 视图
```

## 6. 与 C 的对照（呼应 char 数组讲义）

C 语言里**没有** substr——这是 C++ string 的"自动挡"优势。C 里要切子串得自己动手：

```c
// C：切出 src 从 pos 开始的前 count 个字符
char buf[32] = {0};
strncpy(buf, src + pos, count);   // 手动"指针偏移 + 复制"
```
所以 C 课程里"处理字符串"比 C++ 麻烦得多——你从 C++ 学起再学 C，正好体会 string 替你做了什么。

## 7. 中文警告（项目里踩过的人不少）

`substr` **按字节切，不是按字符**。UTF-8 编码下一个汉字占 **3 字节**：

```cpp
string zh = "你好世界";     // zh.size() == 12（4 个汉字 × 3 字节）
zh.substr(0, 3);            // "你" —— 恰好 3 字节，正常
zh.substr(0, 1);            // 半个汉字 → 乱码！（别这么切）
```
竞赛里处理的都是 ASCII（数字/字母），没这个问题；项目里处理中文时，先想到"字节 vs 字符"。

## 8. 快查卡 + 常见笔试题

```
s.substr(pos)           从 pos 到结尾
s.substr(pos, count)    从 pos 切 count 个（区间 [pos, pos+count)）
pos == size()           返回空串（合法）
pos >  size()           抛 out_of_range（崩溃）
count 超长              自动截到结尾（不报错）
返回                    新字符串（拷贝，O(count)）
```

**常见笔试题**：
1. `"Hello".substr(2, 2)` 输出？→ `"ll"`
2. `"abc".substr(3)` 输出？→ `""`（不报错）
3. `"abc".substr(4)` 会怎样？→ 抛异常
4. `string s="a,b,c"` 用 `find(',')` + `substr` 依次取出 "a" "b" "c"。

---

**一句话总结**：`substr(pos, count)` = 切 `[pos, pos+count)` 这段，**count 是长度不是下标**；记住"pos 越界抛异常、count 超长自动截"，再配上 `find` 就是万能字符串分割器（P1553 已验证）。
