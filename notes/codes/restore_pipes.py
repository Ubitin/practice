# -*- coding: utf-8 -*-
# 精确还原被误转义的分隔竖线
import io

p = r"D:\lenovo\chat_with_deepseek_harness\讲义\00-知识索引与复习路线图.md"
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

targets = [11, 12, 13, 14, 15, 554]
for ln in targets:
    i = ln - 1
    old = lines[i]
    # 只把"空格 \| 空格"或"\|---"这类【分隔用】的 \| 还原；保留 \|x\| 这种数学记法
    new = old
    new = new.replace(" \\| ", " | ")          # 分隔符（两边有空格）
    new = new.replace("\\|---", "|---")        # 表头分隔行
    new = new.replace("---\\|", "---|")
    if new != old:
        lines[i] = new
        print("L%d 已还原" % ln)

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
print("\n表格块 =", len(blocks), " 列数不齐 =", len(bad))
for b in bad:
    print("   L%d: %s" % (b["start"], dict(b["cols"])))

print("\n还原后的阶段总览表：")
for i in range(10, 15):
    print("  L%d: %s" % (i + 1, lines[i][:70]))
print("\n日志行 L554 开头：", lines[553][:70])
