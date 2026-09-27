# -*- coding: utf-8 -*-
# 手册新增 §12.2「教材例题筛选法（闭卷5分钟法 + 取舍原则）」
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("## 誊抄建议"))
print("anchor L%d: %s" % (i + 1, lines[i]))

blk = [
 '### 12.2 教材例题筛选法：哪些题该做、哪些该跳',
 '```text',
 '【唯一判据】这道题暴露的是「知识缺口」还是「重复舒适区」？',
 '    你已经会的章节 → 只读思路、不做题（最多每章挑 1 道综合题验手感）',
 '    你不会的知识点   → 必做，而且同一知识点做 2~3 道（1 道不够形成手感）',
 '',
 '【可操作判据：闭卷 5 分钟法】',
 '    合上书，白纸写"这个知识点的模板骨架"：',
 '      写得出来                    → 会  → 该章例题跳过',
 '      卡在"参数/边界/转移方程"    → 不会 → 做该章例题 3~5 道',
 '    ⚠️ 判据要落在"能不能写出完整可运行版本"，不是"看懂了"',
 '',
 '【方式比"做不做"更重要】',
 '    ✘ 不限时 + 卡住就翻题解 + 看完抄一遍   → 知识面涨，可靠性不涨（舒适区重复）',
 '    ✔ 限时 + 先自己写 + 卡 25~35 分钟才看题解 + 看完闭卷重写 → 两个都涨',
 '    ⇒ 每题做完写一行台账：用时 / 结果 / 归因（知识缺口 · 严谨性 · 思路没想到）',
 '',
 '【教材 vs 平台刷题的分工】',
 '    书例题：围绕一个知识点建立"这类题怎么想"的框架（不限时）',
 '    CF 题：难度随机 + 限时 + Hack 数据，练"压力下调用框架"',
 '    顺序：先做书例题建框架 → 立刻用 2~3 道 CF 同难度题限时验证',
 '',
 '【两本书去重】同一知识点两本都有 ⇒ 只做一次，以先读到的为准；另一本只扫"有没有新写法"',
 '【时间不够时的砍单顺序】树形DP > DP优化 > 树上路径 > 并查集 > ★单调栈/队列（绝不砍）',
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
