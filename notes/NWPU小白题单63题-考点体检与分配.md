# NWPU XCPC 小白题单（vjudge 11277）· 63 题逐题体检与分配

> **触发点**：你说"还有一个小白提单（[vjudge 11277](https://vjudge.net/article/11277)），进度 37/63，上面很多题是 CF 上的，应该怎么分配"。
> **我已抓取全量数据**：题单名 **「NWPU XCPC 小白题单」**，共 **63 题**，来源分布如下——
> **CodeForces 29 · OpenJ_NOI 11 · SPOJ 8 · AtCoder 5 · UESTC 3 · LibreOj/UniversalOJ/DMOJ/Kattis/Toph/Gym/Baekjoon 各 1**。
> **一句话结论**：**这份题单值得做完（考点覆盖很正），但它的考点集中在"基础套路"，恰好没有覆盖你 NWPUPC 卡住的那三块（树上的问题）。所以分配方式是：题单当"限时练手"快速清掉，同时并行补你的三个缺口。**
> ⚠️ 另：进度 37/63 是你报的，我**无法从题单页读到你已做哪些**（vjudge 进度在你的登录态里）⇒ 本文给**逐题体检表**，你自己划掉已做的。

---

## 一、先看这份题单的考点分布（我逐题分类）

| 考点 | 题数 | 代表题 | 对你 |
|---|---|---|---|
| 基础输出 / 模拟 / 输入输出 | ~10 | `SPOJ-TEST`(Life, the Universe…)、`SPOJ-CPTTRN1/2/8`(Character Patterns)、`SPOJ-INTEST`、`AtCoder-abc333_a`(Three Threes)、`OpenJ-陶陶摘苹果`、`OpenJ-计算邮资` | ✅ 已会，**限时 5 分钟秒掉** |
| 排序 / 计数 / 去重 | ~5 | `OpenJ-明明的随机数`、`OpenJ-分数线划定`、`LibreOJ-3505 数对`、`CodeForces-1873C` | ✅ 已会（你有"去重排序三连"讲义） |
| **双指针 / 滑动窗口** | 2 | **`UESTC-201 Sliding Window`**（窗口最值！）、`CodeForces-1729D` | ⭐ **正对你的缺口②** |
| 前缀和 / 差分 | 3 | `CodeForces-1901B Chip and Ribbon`、`OpenJ-校门外的树`、`UniversalOJ-260` | ✅ 已会 |
| **二分答案** | 5 | **`CodeForces-1843E Tracking Segments`**、**`CodeForces-1856C To Become Max`**、`DMOJ-Eko`、`OpenJ-River Hopscotch`、`Baekjoon-9881` | ✅ 你已会，**限时练手最合适** |
| **贪心** | 5 | `CodeForces-1876A Helmets`、`CodeForces-1891C Smilo and Monsters`、`CodeForces-1842B Tenzing and Books`、`CodeForces-1476A`、`CodeForces-520B` | ✅ 已会（**限时**刷） |
| **交互题** | 2 | **`CodeForces-727C Guess the Array`**、**`CodeForces-679A Bear and Prime 100`** | ⭐⭐ **正对你 NWPUPC J 的缺口，而且比那题简单** |
| 位运算 / 二进制 | 4 | `CodeForces-768B Code For 1`、`CodeForces-743B Chloe and the sequence`、`CodeForces-1721C`、`CodeForces-1870C Colorful Table` | ⭐ 你有位运算基础（1913C 做过） |
| 数论基础 | 2 | `SPOJ-DIVSUM Divisor Summation`、`CodeForces-1476A` | 简单，可做 |
| 递归 / 分治 / 搜索 | 4 | `OpenJ-算24`、`UESTC-153 虫食算`、`DMOJ` 等 | 可做（分治你已会） |
| 滑动窗口 + 单调队列（**唯一一处**） | 1 | **`UESTC-201`** | ⭐ 补缺口②的入口 |
| 构造 / 思维 | ~12 | `CodeForces-1845C`、`CodeForces-1934C Find a Mine`、`CodeForces-689A Mike and Cellphone`、`CodeForces-1230A`、`CodeForces-1190A`、`AtCoder-abc317_b`、`AtCoder-abc243_c`、`Kattis-climbingworm`、`Toph-#Hashtag` 等 | ⚠️ **这类最练"思路没想到"**，值得花时间 |

### ⚠️⚠️ 这份题单**完全没有**覆盖的三块（正是你 NWPUPC 卡住的）

| 缺失考点 | 对应你 NWPUPC 卡住的题 |
|---|---|
| **树上的问题**（树遍历 / 树上路径 / LCA） | **C**（树上路径 mex） |
| **树形 DP / 树上背包** | **I** |
| **单调栈**（"下一个更大元素"型） | **F**（字典序最小子序列） |

⭐ **所以结论很明确**：**题单要清掉，但它替代不了你 C/F/H/I 的专项攻坚。** 两者要并行。

---

## 二、分配方案：题单只花 25% 的时间

| 你的 26 道剩余题，怎么排 | 建议 |
|---|---|
| **A 类（必做，约 15 题）**：所有 **1400+ 的 CF 题** + `SPOJ-DIVSUM` + `UESTC-201` + 二分答案 5 题 | 这些是"练限时"的最好材料 ⇒ **全部限时 25~35 分钟** |
| **B 类（可选，约 8 题）**：AtCoder A/B、OpenJ 基础题、SPOJ 图案题 | 已会 ⇒ **限时 5~8 分钟秒掉**，只用来"热手"和练手感，不要投入超过 1 小时总量 |
| **C 类（跳或缓）**：`SPOJ-CPTTRN2/8`、`Toph-#Hashtag` 这类纯格式题 | **可跳**（除非你想练 I/O 细节） |

**时间预算**：26 题里真正要花时间的大约 **15 题 × 40 分钟 ≈ 10 小时** ⇒ 分摊到 14 天里就是**每天 45 分钟**，完全可以和"补缺口 + 打比赛"并行。

**⚠️ 关键纪律**：题单的题**必须限时做**。如果你还是"不限时、卡住就翻题解"，那么做完 63 题你的可靠性依然是 1100–1200——**同样的坑，换一批题还会踩**。

---

## 三、⏰ 14 天内的优先级（题单 vs 缺口）

```text
① 单栈/单调队列（缺口②，最便宜）        ← 同时做 UESTC-201 Sliding Window
② 交互题（缺口，NWPUPC J）              ← 同时做 CF 727C + CF 679A（比 J 简单，先建立信心）
③ 二分答案 5 题（题单里最集中的一类）    ← 限时 25 分钟/题，直接练配速
④ 树上路径 / 树形 DP（缺口①③，NWPUPC C/I）← 题单里没有，必须自己找题
⑤ 剩下的 CF 1400 题                      ← 限时刷
```

⭐ **一个高性价比发现**：题单里有 **2 道交互题（CF 727C、CF 679A）**，而你在 NWPUPC 的 **J 题**恰好是交互题 ⇒ **先做这两道（更简单）**，再去补 J，是最短的路径。

---

## 四、63 题逐题体检表（打印出来划）

> **用法**：在"状态"列填 `✔`（已做）/ `—`（跳过）/ 留空（待做）；"限时目标"按 CF 分换算。

| # | 来源 | 题名 | 考点 | 限时目标 | 状态 |
|---|---|---|---|---|---|
| 1 | AtCoder | MissingNo. (abc317_b) | 基础 | 5 min | |
| 2 | CodeForces | Strong Password (1845C) | 构造 | 25 min | |
| 3 | CodeForces | Find a Mine (1934C) | 交互/构造 | 30 min | |
| 4 | OpenJ_NOI | 陶陶摘苹果 | 基础 | 5 min | |
| 5 | CodeForces | Guess the Array (727C) | **交互** ⭐ | 30 min | |
| 6 | CodeForces | Target Practice (1873C) | 模拟 | 10 min | |
| 7 | CodeForces | Tokitsukaze and Discard Items (1190A) | 模拟/数学 | 25 min | |
| 8 | CodeForces | Mike and Cellphone (689A) | 构造 | 20 min | |
| 9 | OpenJ_NOI | 计算邮资 | 基础 | 5 min | |
| 10 | CodeForces | Two Buttons (520B) | 贪心/BFS | 20 min | |
| 11 | AtCoder | Prefix K-th Max (abc234_d) | 堆 | 20 min | |
| 12 | CodeForces | Friends and the Restaurant (1729D) | 贪心+双指针 | 25 min | |
| 13 | LibreOJ | 数对 (3505) | 计数 | 15 min | |
| 14 | SPOJ | Hidden Couple (HCOUPLE) | 数学 | 20 min | |
| 15 | UESTC | 虫食算 (153) | 搜索 | 40 min | |
| 16 | DMOJ | Eko (coci11c5p2) | **二分答案** ⭐ | 25 min | |
| 17 | OpenJ_NOI | 算24 | 搜索 | 30 min | |
| 18 | CodeForces | K-divisible Sum (1476A) | 贪心/数学 | 20 min | |
| 19 | CodeForces | To Become Max (1856C) | **二分答案** ⭐ | 30 min | |
| 20 | CodeForces | Helmets in Night Light (1876A) | 贪心 | 25 min | |
| 21 | AtCoder | Yamanote Line Game (abc244_c) | **交互** ⭐ | 20 min | |
| 22 | CodeForces | Chip and Ribbon (1901B) | 差分 | 25 min | |
| 23 | CodeForces | Game with Multiset (1913C) | **你已做** ✅ | — | ✔ |
| 24 | CodeForces | Dawid and Bags of Candies (1230A) | 枚举 | 10 min | |
| 25 | CodeForces | Maze (377A) | **你已做** ✅ | — | ✔ |
| 26 | CodeForces | Wooden Toy Festival (1840D) | **你已做** ✅ | — | ✔ |
| 27 | Kattis | Climbing Worm | 数学 | 10 min | |
| 28 | UniversalOJ | 玩具谜题 (260) | 模拟 | 15 min | |
| 29 | CodeForces | Place for a Selfie (1805C) | **你已做** ✅ | — | ✔ |
| 30 | CodeForces | Tracking Segments (1843E) | **二分答案** ⭐ | 30 min | |
| 31 | OpenJ_NOI | River Hopscotch | **二分答案** ⭐ | 30 min | |
| 32 | SPOJ | Life, the Universe, and Everything (TEST) | 基础 | 3 min | |
| 33 | OpenJ_NOI | 分数线划定 | 排序 | 10 min | |
| 34 | SPOJ | Book Gift (BOOKGFT) | 贪心 | 20 min | |
| 35 | CodeForces | Smilo and Monsters (1891C) | 贪心 | 30 min | |
| 36 | OpenJ_NOI | 明明的随机数 | 去重排序 | 5 min | |
| 37 | UESTC | 生日蛋糕 (2829) | 搜索/剪枝 | 40 min | |
| 38 | CodeForces | Code For 1 (768B) | 位运算/递归 | 30 min | |
| 39 | CodeForces | Bear and Prime 100 (679A) | **交互+素数** ⭐ | 30 min | |
| 40 | OpenJ_NOI | 笨小猴 | 字符串 | 10 min | |
| 41 | Toph | #Hashtag | 格式化输出 | 10 min | |
| 42 | CodeForces | Bitwise Operation Wizard (1937C) | 交互+位运算 | 40 min | |
| 43 | AtCoder | Three Threes (abc333_a) | 基础 | 3 min | |
| 44 | Gym | ISBN Conversion (104757I) | **你已做** ✅ | — | ✔ |
| 45 | CodeForces | Min-Max Array Transformation (1721C) | 双指针 | 30 min | |
| 46 | CodeForces | Tenzing and Books (1842B) | 贪心/位运算 | 25 min | |
| 47 | CodeForces | Fear of the Dark (1886B) | **你已做** ✅ | — | ✔ |
| 48 | Baekjoon | Ski Course Design (9881) | **二分/枚举** | 25 min | |
| 49 | SPOJ | Enormous Input Test (INTEST) | 快读 | 5 min | |
| 50 | AtCoder | Collision 2 (abc243_c) | 模拟/排序 | 20 min | |
| 51 | CodeForces | Military Problem (1006E) | **你已做** ✅ | — | ✔ |
| 52 | OpenJ_NOI | 金币 | 模拟 | 10 min | |
| 53 | OpenJ_NOI | 不高兴的津津 | 基础 | 5 min | |
| 54 | CodeForces | Colorful Table (1870C) | 位运算/模拟 | 25 min | |
| 55 | CodeForces | Max Median (1486D) | **你已做** ✅ | — | ✔ |
| 56 | SPOJ | Character Patterns Act 1 | 格式化输出 | 10 min | |
| 57 | SPOJ | Divisor Summation (DIVSUM) | 数论 | 20 min | |
| 58 | UESTC | **Sliding Window (201)** | **单调队列** ⭐⭐ | 35 min | |
| 59 | CodeForces | Chloe and the sequence (743B) | 位运算/递归 | 20 min | |
| 60 | OpenJ_NOI | 校门外的树 | 差分/模拟 | 10 min | |
| 61 | SPOJ | Character Patterns Act 2 | 格式化输出 | 10 min | |
| 62 | OpenJ_NOI | 年龄与疾病 | 基础 | 5 min | |
| 63 | SPOJ | Character Patterns Act 8 | 格式化输出 | 10 min | |

**我的记录里已确认你做过 8 题**（23/25/26/29/44/47/51/55，带 ✔）⇒ 你若已做 37 题，**剩余约 26 题**，其中：
- **⭐ 高价值必做（约 15 题）**：2(1845C)、5(727C)、16、19、20、30、31、35、38、39、42、45、46、48、58
- **可跳（约 6 题）**：1、4、9、32、43、53 这类纯基础/纯格式题（已会，最多花 5 分钟热手）
- **其余（约 5 题）**：中间档，有空就做

---

## 五、自查

<details><summary>Q1：为什么说这份题单"覆盖很正"但"不够"？</summary>

**正**：它把 1400±200 最常用的套路都覆盖了——二分答案（5 题）、贪心（5 题）、前缀和/差分（3 题）、双指针/滑窗（2 题）、交互（2 题）、位运算（4 题），**这正是 Div.3 的 D/E 需要的东西**。
**不够**：它**完全没有树上问题**（树遍历/树上路径/树形 DP）和**单调栈**——而这三块恰好是你 NWPUPC 卡住的 C/F/I。⇒ **题单补"广度与配速"，专项攻坚补"缺口"，两者不可互换。**

</details>

<details><summary>Q2：37/63 这个进度算快还是慢？</summary>

按"63 题覆盖了套路面"来看，**37 题已经过了收益率最高的阶段**（基础+模拟+排序+二分答案基本都碰过了）。
剩下的 26 题里，**真正能带来新东西的大约 8~10 题**（见上表 ⭐ 标记）⇒ **不必因为"没做完"焦虑**，把 ⭐ 的做完就可以推进到下一阶段。
⭐ 判据不是"完成度"，而是**"限时一次 AC 的比例"**：如果这 37 题里你有大量是"看完题解抄的"，那实际收益要打折。

</details>

<details><summary>Q3：题单里的交互题（727C、679A）值得优先做吗？</summary>

**非常值得**，原因有三：
1. 你在 NWPUPC 的 **J 题**就是交互（二分猜数），而你**从没做过交互题**——这是"形式型"缺口，不是算法缺口；
2. 这两道**比 J 简单**（J 要求次数 ≤ ⌊log₂N⌋+1，而 727C 是"猜数组"、679A 是"猜素数"）；
3. 交互题的坑**只在形式上**（输出后必须 `fflush(stdout)` / `cout << flush`、不能多输出、要按协议读回复）⇒ 做两道就掌握了。
⇒ **建议放在缺口清单的第 2 位**（仅次于单调栈/队列）。

</details>

<details><summary>Q4：题单里的"构造/思维题"为什么值得花时间？</summary>

因为它们练的是你台账三因里的**"思路没想到"**——这类失分在 Div.3 的 C/D 里最常见，而且**补起来最慢**（没有模板可背）。
你题单里有约 12 道这类题（1845C、1934C、689A、1190A、1230A、abc317_b、abc243_c…）⇒ 做它们的正确方式是：**先自己想 20 分钟**，想不出来**只看题解的第一段**（思路提示），然后**合上题解自己写**。

</details>

<details><summary>Q5：如果只有 14 天，题单和缺口只能选一个，选哪个？</summary>

**选题单的 ⭐ 部分（约 10 题）+ 缺口里的 2 块（单调栈/队列、交互）**，放弃题单里的基础题。
理由：题单基础题你已会（重复劳动），而 ⭐ 题和缺口题都有"限时 + 新东西"双重收益。
⇒ **判据始终是"边际收益"**：已经会的题做 10 道，不如不会的题做 3 道。</details>
