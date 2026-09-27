# -*- coding: utf-8 -*-
# 安全修复：只有当某行的列数与"同块众数列数"不一致时，才把该行的裸竖线转义
import io
from collections import defaultdict

p = r"D:\lenovo\chat_with_deepseek_harness\讲义\00-知识索引与复习路线图.md"
lines = io.open(p, "r", encoding="utf-8").read().split("\n")


def colcount(t):
    return t.replace("\\|", "").count("|") - 1


# 1) 划分表格块，算出每块众数列数
blocks = []
cur = None
for i, l in enumerate(lines):
    t = l.rstrip()
    if t.startswith("|") and t.endswith("|"):
        if cur is None:
            cur = {"rows": []}
        cur["rows"].append(i)
    elif cur is not None:
        blocks.append(cur); cur = None
if cur is not None:
    blocks.append(cur)

fixed = 0
for b in blocks:
    counts = defaultdict(int)
    for i in b["rows"]:
        counts[colcount(lines[i].rstrip())] += 1
    if len(counts) == 1:
        continue                       # 本块本来就一致，不动
    mode = max(counts.items(), key=lambda kv: kv[1])[0]
    print("块 @L%d 众数列数=%d，实际分布=%s" % (b["rows"][0] + 1, mode, dict(counts)))
    for i in b["rows"]:
        t = lines[i].rstrip()
        c = colcount(t)
        if c == mode:
            continue
        # 只转义"裸竖线"：首尾保留，中间未转义的 | 逐个转义
        head, body, tail = t[:1], t[1:-1], t[-1:]
        body = body.replace("\\|", "\x00").replace("|", "\\|").replace("\x00", "\\|")
        lines[i] = head + body + tail
        fixed += 1
        print("   L%d 列数 %d → %d（已转义裸竖线）" % (i + 1, c, colcount(lines[i])))

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("\n共修复", fixed, "行")
