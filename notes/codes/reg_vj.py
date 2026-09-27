# -*- coding: utf-8 -*-
# 索引登记：NWPU 小白题单 讲义行 + 日志 + 收录数
import io, os, glob, re

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
expect = len(glob.glob(os.path.join(d, "*.md"))) - 2

lines = io.open(p, "r", encoding="utf-8").read().split("\n")
lines[4] = re.sub(r"收录：\*\*\d+ 份\*\*", "收录：**" + str(expect) + " 份**", lines[4])

h6 = next(i for i, l in enumerate(lines) if l.startswith("### 线 6"))
h7 = next(i for i, l in enumerate(lines) if l.startswith("### 线 7"))
last6 = max(i for i in range(h6, h7) if lines[i].startswith("| `"))
row = (
 "| `NWPU小白题单63题-考点体检与分配` | **回答「vjudge 小白题单（article/11277）进度 37/63，怎么分配」**：⭐ **已抓取题单全量数据并逐题分类**——题单名 **「NWPU XCPC 小白题单」共 63 题**，来源 **CodeForces 29 · OpenJ_NOI 11 · SPOJ 8 · AtCoder 5 · UESTC 3 · 其余各 1**。"
 "**考点分布**：基础输出/模拟 ~10、排序计数 ~5、**二分答案 5**（1843E/1856C/Eko/River Hopscotch/9881）、**贪心 5**（1876A/1891C/1842B/1476A/520B）、前缀和差分 3、位运算 4、**交互 2**（**727C Guess the Array**、**679A Bear and Prime 100**）、双指针滑窗 2（**UESTC-201 Sliding Window**）、搜索 4、构造思维 ~12。"
 "⚠️⚠️ **但这份题单完全没有覆盖三块**——**树上问题（遍历/路径/LCA）、树形 DP、单调栈** —— 而这正是你 NWPUPC 卡住的 **C/F/I**。⇒ 结论：**题单补"广度与配速"，不能替代缺口攻坚，两者必须并行**。"
 "**分配方案**：题单只花 25% 时间——A 类必做（约 15 题）全部限时 25~35 分钟；B 类基础题限时 5~8 分钟秒掉（总投入≤1 小时）；C 类纯格式题可跳。**26 题里真正要花时间的约 15 题 × 40 min ≈ 10 小时 ⇒ 摊到 14 天每天 45 分钟**。"
 "⭐ **高性价比发现**：题单里 2 道交互题比 NWPUPC 的 J 更简单 ⇒ **先做 727C、679A 再补 J**，是最短路径。**给出 63 题逐题体检表**（来源/题名/考点/限时目标/状态列），并标注我记录里**已确认你做过的 8 题**（1913C、377A、1840D、1805C、Gym-ISBN、1886B、1006E、1486D）。"
 "**14 天优先级**：①单调栈/队列 + UESTC-201 → ②交互（727C/679A）→ ③二分答案 5 题限时 → ④树上路径/树形 DP（题单里没有，自己找题）→ ⑤其余 CF 1400 限时刷。**⚠️ 强调：题单的题必须限时做**，否则做满 63 题可靠性仍是 1100–1200（同样的坑换批题还会踩） |"
)
lines.insert(last6 + 1, row)

log = (
 "| 2026-09-28 | +`NWPU小白题单63题-考点体检与分配`（**线 6 · 综合**，回答「vjudge article/11277 小白题单进度 37/63 怎么分配」）："
 "**方法**：vjudge 页面走 HTTPS 抓取（`Invoke-WebRequest` 报 TLS 失败，改用 Python + `CERT_NONE` 成功），从内嵌 `problemsBrief` JSON 提取全量 63 题并逐题按考点分类。"
 "**题单构成**：CF 29 / OpenJ_NOI 11 / SPOJ 8 / AtCoder 5 / UESTC 3 / 其余 7 家各 1。"
 "**核心结论**：考点覆盖了 Div.3 的 D/E 常用套路（二分答案 5、贪心 5、差分 3、滑窗 2、交互 2、位运算 4），"
 "**但完全没有树上的问题（遍历/路径/LCA）、树形 DP、单调栈** —— 正是用户 NWPUPC 卡住的 C/F/I ⇒ **题单补广度与配速，不能替代缺口攻坚**。"
 "**给出 63 题逐题体检表**（含限时目标与状态列）与**三类分配**（必做 ⭐15 / 基础秒掉 / 可跳），"
 "并**交叉核对出我记录里已确认做过的 8 题**；**时间预算** 15 题×40min≈10h ⇒ 每天 45 分钟即可并行。"
 "**发现**：题单里 2 道交互题（727C/679A）比 NWPUPC 的 J 简单 ⇒ 优先做它们再补 J。"
 "**⚠️ 强调纪律**：题单题必须限时做，否则清满 63 题可靠性仍是 1100–1200。"
 "（本轮为纯规划与清单，**未改动手册**，故未重跑 docx）→ 收录数 **117 → " + str(expect) + " 份** |"
)
last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("done, lines =", len(lines), " 收录 =", expect)
