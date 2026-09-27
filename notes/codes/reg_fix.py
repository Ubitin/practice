# -*- coding: utf-8 -*-
# 在索引日志追加一条：本轮格式修复记录
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

log = (
 "| 2026-09-27 | **手册格式体检与修复**（在「组合数」知识点入库时顺手做的）：用脚本逐行跟踪代码块围栏，查出手册里有"
 "**两处代码块从头就没闭合**（§3.16 二分答案+周期位移前缀和、§6.14 k 的倍数）——后果是**其后所有 `##`/`###` 标题都被吞进代码块**，"
 "打印版里会整段变成代码样式。修复后：**围栏 152 → 154（偶数、全部闭合）**、**「落在代码块内的标题」29 → 0**、"
 "`check_width.py` 报告的代码行数从 781 跳到 **1643**（说明之前大量正文被误算进代码块）、docx 从 138740 涨到 **139417 字节**；"
 "并用 `python-docx` 逐个段落核对：`6.15`/`6.16` 等新小节确实是独立的 **Heading 3** 段落（不是粘在代码块里）。"
 "⚠️ 教训：**批量为手册插入小节后，一定要跑一次「围栏配对 + 标题是否在块内」的体检**——这类错误不影响阅读 markdown，"
 "但在**打印版里会整段变形**，而且不会报错，属于静默损坏；本轮工具已留下 `test_\\chk_blocks.py` 与 `test_\\fix_blocks.py` 可复用 |"
)
last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("日志已追加，总行数 =", len(lines))
