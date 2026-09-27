# -*- coding: utf-8 -*-
# 精确修复 L555：只还原日期后的分隔竖线，其余保持转义（正文里那些是数学/代码记法）
import io

p = r"D:\lenovo\chat_with_deepseek_harness\讲义\00-知识索引与复习路线图.md"
lines = io.open(p, "r", encoding="utf-8").read().split("\n")
i = 554

old = lines[i]
print("修复前列数 =", old.replace("\\|", "").count("|") - 1)

# 只处理开头的 "| 2026-09-27 \| " → "| 2026-09-27 | "
new = old.replace("| 2026-09-27 \\| ", "| 2026-09-27 | ", 1)
lines[i] = new
print("修复后列数 =", new.replace("\\|", "").count("|") - 1)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

# 体检
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
print("表格块 =", len(blocks), " 列数不齐 =", len(bad))
for b in bad:
    print("   L%d: %s" % (b["start"], dict(b["cols"])))
