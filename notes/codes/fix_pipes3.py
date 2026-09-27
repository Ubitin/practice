# -*- coding: utf-8 -*-
# 只对 L554 那一行做精确的竖线转义
import io
from collections import defaultdict

p = r"D:\lenovo\chat_with_deepseek_harness\讲义\00-知识索引与复习路线图.md"
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = 553                       # L554
old = lines[i]
print("修复前：列数 =", old.replace("\\|", "").count("|") - 1)

new = old
# 精确替换（逐一针对裸竖线，保留已有的 \| 与 {2:2, 3:2, 7:1} 里的数学竖线写法）
pairs = [
    ("`d | a` 与", "`d \\| a` 与"),
    ("「行尾的 `|`」", "「行尾的 `\\|`」"),
    ("任何 `|` 都必须转义", "任何 `\\|` 都必须转义"),
    ("（列数 `{2:2, 3:2, 7:1}`）", "（列数 `{2:2, 3:2, 7:1}`）"),   # 这处其实是花括号，无竖线
]
for a, b in pairs:
    new = new.replace(a, b)

lines[i] = new
print("修复后：列数 =", new.replace("\\|", "").count("|") - 1)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

# 体检
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
