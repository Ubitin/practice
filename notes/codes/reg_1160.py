# -*- coding: utf-8 -*-
# 索引登记：P1160 讲义行（线 2）+ 日志 + 收录数
import io, os, glob, re

D = r"D:\lenovo\chat_with_deepseek_harness"
d = os.path.join(D, "讲义")
p = os.path.join(d, "00-知识索引与复习路线图.md")
expect = len(glob.glob(os.path.join(d, "*.md"))) - 2

row = io.open(os.path.join(D, "test_", "row_1160.txt"), "r", encoding="utf-8").read().strip("\n")
log = io.open(os.path.join(D, "test_", "log_1160.txt"), "r", encoding="utf-8").read().strip("\n")

lines = io.open(p, "r", encoding="utf-8").read().split("\n")
lines[4] = re.sub(r"收录：\*\*\d+ 份\*\*", "收录：**" + str(expect) + " 份**", lines[4])

# 线 2 表尾
h2 = next(i for i, l in enumerate(lines) if l.startswith("### 线 2"))
h3 = next(i for i, l in enumerate(lines) if l.startswith("### 线 3"))
last2 = max(i for i in range(h2, h3) if lines[i].startswith("| `"))
lines.insert(last2 + 1, row)

last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("done, lines =", len(lines), " 收录 =", expect)

from collections import defaultdict
cur = None
blocks = []
for j, l in enumerate(lines):
    t = l.rstrip()
    if t.startswith("|") and t.endswith("|"):
        c = t.replace("\\|", "").count("|") - 1
        if cur is None:
            cur = {"start": j + 1, "cols": defaultdict(int)}
        cur["cols"][c] += 1
    elif cur is not None:
        blocks.append(cur); cur = None
if cur is not None:
    blocks.append(cur)
bad = [b for b in blocks if len(b["cols"]) > 1]
print("表格块 =", len(blocks), " 列数不齐 =", len(bad), [b["start"] for b in bad])
