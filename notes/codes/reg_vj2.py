# -*- coding: utf-8 -*-
# 索引登记（安全版：行内容从单独文件读取，避免引号冲突）
import io, os, glob, re

D = r"D:\lenovo\chat_with_deepseek_harness"
d = os.path.join(D, "讲义")
p = os.path.join(d, "00-知识索引与复习路线图.md")
expect = len(glob.glob(os.path.join(d, "*.md"))) - 2

# 从文件读入两段文本（每段一行，不含换行）
row = io.open(os.path.join(D, "test_", "row_vj.txt"), "r", encoding="utf-8").read().strip("\n")
log = io.open(os.path.join(D, "test_", "log_vj.txt"), "r", encoding="utf-8").read().strip("\n")

lines = io.open(p, "r", encoding="utf-8").read().split("\n")
lines[4] = re.sub(r"收录：\*\*\d+ 份\*\*", "收录：**" + str(expect) + " 份**", lines[4])

h6 = next(i for i, l in enumerate(lines) if l.startswith("### 线 6"))
h7 = next(i for i, l in enumerate(lines) if l.startswith("### 线 7"))
last6 = max(i for i in range(h6, h7) if lines[i].startswith("| `"))
lines.insert(last6 + 1, row)

last = max(i for i, l in enumerate(lines) if l.startswith("| 2026-"))
lines.insert(last + 1, log)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("done, lines =", len(lines), " 收录 =", expect)
