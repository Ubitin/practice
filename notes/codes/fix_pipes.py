# -*- coding: utf-8 -*-
# 修复：把该讲义行里的裸竖线转义（否则会把 2 列表格撑破）
import io, os, glob
from collections import defaultdict

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

key = "| `辗转相除求gcd-那行while在干什么`"
i = next(i for i, l in enumerate(lines) if l.startswith(key))
old = lines[i]
print("修复前 L%d 竖线数 = %d" % (i + 1, old.replace("\\|", "").count("|") - 1))

new = old
# 数学记法里的裸竖线 → 转义
new = new.replace("`d | a 且 d | b ⇒ d | (a%b)`", "`d \\| a 且 d \\| b ⇒ d \\| (a%b)`")
new = new.replace("`gcd(x,0)=|x|`", "`gcd(x,0)=\\|x\\|`")
# 兜底：把" | "（两边带空格的裸竖线）也转义，但别碰行首/行尾与分隔符
body_start = new.index("|", 1) + 2          # 越过开头的 "| `名字` | "
head, body = new[:body_start], new[body_start:]
body = body.replace(" | ", " \\| ")
# 收尾的那个表格分隔符要还原
new = head + body

# 确保行尾仍是一个未转义的 |
if not new.endswith("|") or new.endswith("\\|"):
    new = new.rstrip("\\|").rstrip() + " |"

lines[i] = new
print("修复后 L%d 竖线数 = %d" % (i + 1, new.replace("\\|", "").count("|") - 1))

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
    print("   ⚠️ L%d: %s" % (b["start"], dict(b["cols"])))
print("总行数 =", len(lines))
