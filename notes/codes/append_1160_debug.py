# -*- coding: utf-8 -*-
# ① 讲义追加第九节（排错实录） ② 手册 §3.18 末尾补两条 ⚠️
import io, os, glob

D = r"D:\lenovo\chat_with_deepseek_harness"
d = os.path.join(D, "讲义")

# ---------- ① 讲义 ----------
lec = [f for f in glob.glob(os.path.join(d, "*.md")) if "数组双向链表" in os.path.basename(f)][0]
add = """

---

## 九、排错实录：你的 P1160 为什么 WA（**两个插入函数各少一行**）

> **触发点**：你按上面的结构手敲了 P1160，交上去 WA。
> **一句话结论**：`ins_back` 与 `ins_front` **各少最后一句"回指"**，导致**新节点挂上去了、但 `now` 不知道它的存在** ⇒ 从起点往右走会**跳过所有新插入的节点**。

### 9.1 实测数据

| 验证 | 结果 |
|---|---|
| 官方样例 `4 / 1 0 / 2 1 / 1 0 / 2 / 3` | 输出 `2 4 1` —— **居然是对的**（见 9.3 解释） |
| **vs 参考实现，500 组随机合法数据** | **不一致 462 组** ✘ |
| 首个反例 | 你的输出 `2 5 1`，正确 `2 12 11 5 13 8 3 1` |

### 9.2 缺的是哪两行

```cpp
void ins_back(ll x,ll y){
    ll now = indexx[x];
    a[++tot] = {y,now,a[now].nxt};
    a[a[now].nxt].pre = tot;
    // ❌ 少了 a[now].nxt = tot;
    indexx[y] = tot;
}
void ins_front(ll x,ll y){
    ll now = indexx[x];
    a[++tot] = {y,a[now].pre,now};
    a[a[now].pre].nxt = tot;
    // ❌ 少了 a[now].pre = tot;
    indexx[y] = tot;
}
```

**插入一个节点要改"两边的两对"指针**：

| 插入到 x 右边，需要改 4 个指针 | 你写了 | 说明 |
|---|---|---|
| 新节点的 `pre` ← now | ✅ | 构造函数里 |
| 新节点的 `nxt` ← 原右邻 | ✅ | 构造函数里 |
| 原右邻的 `pre` ← 新节点 | ✅ | `a[a[now].nxt].pre = tot;` |
| **now 的 `nxt` ← 新节点** | ❌ | **漏了** |

⇒ 后果：`now` 的右指针**仍然指向原来的右邻**，新节点成了"**只有人指向它、它不被人指向**"的孤岛（其实是被跳过）⇒ 遍历时**整批丢失**。

### 9.3 为什么样例能骗过你

样例的操作是 `2 插到 1 左`、`3 插到 1 右`、`4 插到 1 左` —— **三个操作全部作用在起点 1 上**，
而 1 的左右指针**恰好被另一个缺了行的函数补上了**（`ins_front` 改的是 `a[a[now].pre].nxt`，当 `now=1`、`a[1].pre=0` 时改的正是哨兵的 `nxt`）⇒ 两处缺失**互相掩盖**。
🔴 **这正是"样例全对、大面积 WA"的典型模式**：**样例只覆盖了一小部分代码路径**。

### 9.4 修正与验证

补两行即可（顺序不变，你原来的顺序本来就对）：

```cpp
a[a[now].nxt].pre = tot;
a[now].nxt = tot;        // ★ 补：now 的右指针 → 新节点（ins_back）
...
a[a[now].pre].nxt = tot;
a[now].pre = tot;        // ★ 补：now 的左指针 → 新节点（ins_front）
```

**实测**：补两行后 —— 官方样例 `2 4 1` ✅、**1000 组随机对拍不一致 0** ✅。
（未改动你的 `01.cpp`；修正版在 `test_\\01_p1160_fixed.cpp`。）

### 9.5 这一类 bug 的自查法（**改指针时数数**）

> 在链表里"插入一个节点"永远要改 **4 个指针**（新节点 2 个 + 两侧邻居各 1 个）；
> "删除一个节点"永远要改 **2 个指针**（左右邻居互相指过去）。
> **改完以后数一数自己写了几句**——少一句就是断链。
> ⭐ 更稳的做法：**用对拍**（对拍 20 组就能抓到 462/500 这种量级的错，而样例完全看不出来）。
"""

io.open(lec, "a", encoding="utf-8").write(add)
print("讲义已追加，大小 =", os.path.getsize(lec))

# ---------- ② 手册 ----------
hb = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(hb, "r", encoding="utf-8").read().split("\n")
i = next(i for i, l in enumerate(lines) if l.startswith("### 3.18 数组模拟双向链表"))
# 找到该节代码块结束的那一行（下一个 ``` 之后）
j = i
while not lines[j].lstrip().startswith("```"):
    j += 1
j += 1
while not lines[j].lstrip().startswith("```"):
    j += 1
ins = [
 '// ⚠️⚠️ 用户实测 WA 实录（2026-09-28）：两个插入函数【各少最后一句回指】——',
 '//      ins_back 少 a[now].nxt = tot;   ins_front 少 a[now].pre = tot;',
 '//      后果：新节点挂上去了，但 now 不知道它存在 ⇒ 从起点往右走会【跳过所有新插入的节点】',
 '//      实测：官方样例居然过（样例三个操作都作用在起点 1 上，两处缺失互相掩盖），',
 '//            但 vs 参考实现 500 组随机数据【错 462 组】（你的 2 5 1 vs 正确 2 12 11 5 13 8 3 1）',
 '//      ⇒ 铁律：链表【插入一个节点永远改 4 个指针】（新节点 2 个 + 两侧邻居各 1 个）；',
 '//              链表【删除一个节点永远改 2 个指针】（左右邻居互相指过去）。改完数一数写了几句！',
 '//      ⇒ 这种错【样例完全看不出来】，只能靠对拍（20 组就够）。',
]
for k, s in enumerate(ins):
    lines.insert(j, s)
io.open(hb, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("手册已在 §3.18 代码块内补 7 行，总行数 =", len(lines))

fences = [k for k, l in enumerate(lines) if l.lstrip().startswith("```")]
inside = False; bad = 0
for l in lines:
    if l.lstrip().startswith("```"):
        inside = not inside; continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad += 1
print("围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0, " 块内标题 =", bad)
