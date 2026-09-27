# -*- coding: utf-8 -*-
# 精确修复：删掉手册末尾那个重复的闭合围栏
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")


def fence_state(ls):
    inside = False
    total = 0
    for l in ls:
        if re.match(r"^\s*```", l):
            total += 1
            inside = not inside
    return total, inside


t0, in0 = fence_state(lines)
print("修复前：行数=%d 围栏=%d 未闭合=%s" % (len(lines), t0, in0))

# 目标：L2097（索引 2096）那一行是重复的 ```
idx = 2096
print("检查 L2097 内容 =", repr(lines[idx]))
assert re.match(r"^\s*```\s*$", lines[idx]), "L2097 不是围栏行，中止"
assert re.match(r"^\s*```\s*$", lines[idx - 1]), "L2096 不是围栏行，中止"

del lines[idx]

t1, in1 = fence_state(lines)
print("修复后：行数=%d 围栏=%d 未闭合=%s" % (len(lines), t1, in1))

# 重新检查"标题落在代码块内"
inside = False
open_at = 0
bad = []
for i, l in enumerate(lines):
    if re.match(r"^\s*```", l):
        if inside:
            inside = False
        else:
            inside = True
            open_at = i + 1
        continue
    if inside and re.match(r"^#{2,3} ", l):
        bad.append((open_at, i + 1, l[:50]))
print("标题掉进代码块的次数 =", len(bad))
for a, b, t in bad[:10]:
    print("   L%d 开启 → L%d: %s" % (a, b, t))

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("已写回")
