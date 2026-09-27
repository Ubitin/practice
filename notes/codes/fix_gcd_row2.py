# -*- coding: utf-8 -*-
# 修正：把误插进线 6 表的两行检索记录，移到「四、快速检索表」里
import io, os, glob
from collections import defaultdict

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = os.path.join(d, "00-知识索引与复习路线图.md")
lines = io.open(p, "r", encoding="utf-8").read().split("\n")


def blocks_of(ls):
    res = []
    cur = None
    for i, l in enumerate(ls):
        t = l.rstrip()
        if t.startswith("|") and t.endswith("|"):
            c = t.replace("\\|", "").count("|") - 1
            if cur is None:
                cur = {"start": i + 1, "cols": defaultdict(int), "idx": []}
            cur["cols"][c] += 1
            cur["idx"].append(i)
        elif cur is not None:
            res.append(cur); cur = None
    if cur is not None:
        res.append(cur)
    return res


print("修复前：")
for b in blocks_of(lines):
    if len(b["cols"]) > 1:
        print("  ⚠️ L%d 列数不齐: %s" % (b["start"], dict(b["cols"])))

# 1) 取出两行"最大公约数/最小公倍数"的检索行
keys = ("| 最大公约数怎么写", "| 最小公倍数怎么算")
picked = []
for k in keys:
    i = next(i for i, l in enumerate(lines) if l.startswith(k))
    picked.append(lines.pop(i))
print("取出 2 行检索记录")

# 2) 插到「四、快速检索表」表尾（该表最后一个以 | 开头的行之后）
h4 = next(i for i, l in enumerate(lines) if l.startswith("## 四、快速检索表"))
h5 = next(i for i, l in enumerate(lines) if l.startswith("## 五、"))
last = max(i for i in range(h4, h5) if lines[i].rstrip().startswith("|"))
print("检索表表尾 L%d" % (last + 1))
for k, r in enumerate(picked):
    lines.insert(last + 1 + k, r)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

print("\n修复后：")
bad = 0
for b in blocks_of(lines):
    if len(b["cols"]) > 1:
        print("  ⚠️ L%d 列数不齐: %s" % (b["start"], dict(b["cols"])))
        bad += 1
print("  列数不齐的表格块 =", bad, " 总行数 =", len(lines))
