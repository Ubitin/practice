# -*- coding: utf-8 -*-
# 修正：把误插进检索表的「辗转相除求gcd」讲义行移到线 6 表尾
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

# 1) 摘出该行
key = "| `辗转相除求gcd-那行while在干什么`"
idx = next(i for i, l in enumerate(lines) if l.startswith(key))
row = lines.pop(idx)
print("从 L%d 取出该行" % (idx + 1))

# 2) 找到线 6 表尾：最后一个以 "| `" 开头、且位于线6与线7之间的行
h6 = next(i for i, l in enumerate(lines) if l.startswith("### 线 6"))
h7 = next(i for i, l in enumerate(lines) if l.startswith("### 线 7"))
last_row = max(i for i in range(h6, h7) if lines[i].startswith("| `"))
print("线6 表尾 L%d: %s" % (last_row + 1, lines[last_row][:40]))
lines.insert(last_row + 1, row)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

# 3) 体检
inside = False
bad = []
for i, l in enumerate(lines):
    if l.startswith("|") and l.rstrip().endswith("|"):
        raw = l.replace("\\|", "")
        bad.append((i + 1, raw.count("|") - 1))
from collections import defaultdict
# 按块检查列数
blocks = []
cur = None
for i, l in enumerate(lines):
    t = l.rstrip()
    if t.startswith("|") and t.endswith("|"):
        c = t.replace("\\|", "").count("|") - 1
        if cur is None:
            cur = {"start": i + 1, "cols": defaultdict(int)}
        cur["cols"][c] += 1
    elif cur is not None:
        blocks.append(cur); cur = None
if cur is not None:
    blocks.append(cur)
badb = [b for b in blocks if len(b["cols"]) > 1]
print("表格块 =", len(blocks), " 列数不齐 =", len(badb))
for b in badb:
    print("   ⚠️ L%d: %s" % (b["start"], dict(b["cols"])))
