# -*- coding: utf-8 -*-
# 找出并修掉新日志行里的裸竖线
import io, os, glob
from collections import defaultdict

p = r"D:\lenovo\chat_with_deepseek_harness\讲义\00-知识索引与复习路线图.md"
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

# 找列数异常的行
for i, l in enumerate(lines):
    t = l.rstrip()
    if t.startswith("|") and t.endswith("|"):
        c = t.replace("\\|", "").count("|") - 1
        if c != 2 and c != 3:
            print("可疑 L%d 列数=%d" % (i + 1, c))
            idx = 0
            pos = []
            for ch in t:
                if ch == "|":
                    pos.append(idx)
                idx += 1
            for pp in pos:
                s = max(0, pp - 28)
                print("    @%d: ...%s..." % (pp, t[s:s + 56]))

# 自动修复：把该行里非首尾的裸竖线转义
fixed = 0
for i, l in enumerate(lines):
    t = l.rstrip()
    if not (t.startswith("|") and t.endswith("|")):
        continue
    c = t.replace("\\|", "").count("|") - 1
    if c in (2, 3):
        continue
    # 保留首尾两个分隔符，中间的裸 | 全部转义
    head = t[:1]
    tail = t[-1:]
    body = t[1:-1].replace("\\|", "\x00")     # 保护已有的转义
    body = body.replace("|", "\\|")
    body = body.replace("\x00", "\\|")
    lines[i] = head + body + tail
    fixed += 1
    print("已修复 L%d → 列数 %d" % (i + 1, lines[i].replace("\\|", "").count("|") - 1))

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("共修复", fixed, "行，总行数 =", len(lines))
