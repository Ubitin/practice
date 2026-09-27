# -*- coding: utf-8 -*-
# 手册新增 §12.1「Div.3 准备度自测题 + 判据」（放在 §12 难度标尺之后）
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

# 锚点：## 誊抄建议（§12 之后的收尾节）
i = next(i for i, l in enumerate(lines) if l.startswith("## 誊抄建议"))
print("anchor L%d: %s" % (i + 1, lines[i]))

blk = [
 '### 12.1 准备度自测：3 道题 + 判据表（能不能开打 Div.3）',
 '```text',
 '【怎么用】开秒表，独立完成，不看题解。记录：用时 / 是否一次 AC / 若 WA 错在哪。',
 '',
 '【题 1 · 等值对计数 ≈1400】',
 '  给 n 个整数（可负可零），求有多少对 i<j 满足 a[i]+a[j]==0。n ≤ 2e5，|a[i]| ≤ 1e9',
 '  样例1: 5 / -1 1 2 -2 0        -> 2',
 '  样例2: 6 / -1 1 2 -2 0 0      -> 3      ← 第二组专考重复值',
 '  关键点: map 计数或排序后 lower_bound 数次数；',
 '          ⚠️ 0 与 0 配对贡献 C(cnt,2)=cnt(cnt-1)/2（不是 cnt²）；每个值只数一侧防重复',
 '',
 '【题 2 · 容器灌水 ≈1400】',
 '  n 个容器初始水量 a[i]，总加水不超过 w（最终水量须为整数），问最小水位最大能到多少。',
 '  多组，n ≤ 2e5，w ≤ 1e18，a[i] ≤ 1e9',
 '  样例: 1 / 4 7 / 1 1 1 1       -> 2',
 '  关键点: 二分答案，判定 Σ max(0, x-a[i]) ≤ w；',
 '          ⚠️ 上界给足、求和用 __int128、求最大可行值时 mid 要上取整 lo+(hi-lo+1)/2',
 '',
 '【题 3 · 区间改值后的总和奇偶 ≈1400】',
 '  n 个数，q 次询问给 l,r,k：把 [l,r] 全改成 k 后整个数组的和是否为奇数（只问奇偶）。',
 '  多组，n,q ≤ 2e5，a[i],k ≥ 1',
 '  样例: 1 / 3 2 / 1 2 3 / 1 2 4 / 2 3 1  -> YES / YES',
 '  关键点: 前缀和 + 奇偶：新总和 = pre[n] - (pre[r]-pre[l-1]) + k*(r-l+1)；',
 '          ⚠️ 别真改数组（O(nq)）；pre[n] 存下来别每次重算；注意 int 溢出',
 '',
 '【判据表（对完答案填）】',
 '  3 道里 ≥2 道 25 分钟内一次 AC   -> ✅ 已达 Div.3 入场券，直接开打',
 '  3 道里 1 道达标                -> ⚠️ 瓶颈是可靠性：刷 20 道 1300-1400 限时题，别学新算法',
 '  全没达标但想得到思路           -> ⚠️ 严谨性问题：编译带 -Wall + 自检四问（数组/初值/边界/最大数据）',
 '  连思路都想不到                 -> ❌ 回去补工具（排序+二分 / 二分答案 / 前缀和）',
 '',
 '【Div.3 该练/别碰】',
 '  ✅ 练: 贪心+排序、前缀和/差分、二分答案、BFS/DFS 变形、简单 DP、单调栈/单调队列',
 '  ❌ 别碰: 线段树、树状数组、最短路、拓扑/强连通、树上问题、区间/状压/数位 DP（要 1900+）',
 '```',
 ''
]
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

fences = [j for j, l in enumerate(lines) if l.lstrip().startswith("```")]
inside = False
bad = 0
for l in lines:
    if l.lstrip().startswith("```"):
        inside = not inside
        continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad += 1
print("行数 =", len(lines), " 围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0, " 块内标题 =", bad)
