# -*- coding: utf-8 -*-
# 精确诊断：打印每个围栏行的序号与之前的状态，找出错位起点
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

inside = False
seq = 0
events = []
for i, l in enumerate(lines):
    if re.match(r"^\s*```", l):
        seq += 1
        events.append((seq, i + 1, "CLOSE" if inside else "OPEN ", l.strip()))
        inside = not inside

print("总围栏 =", seq, " 偶数 =", seq % 2 == 0, " 最终 inside =", inside)
print("\n前 20 个围栏事件：")
for s, ln, kind, txt in events[:20]:
    print("  #%3d L%-5d %s  %s" % (s, ln, kind, txt[:30]))

# 找出所有"OPEN 紧跟 OPEN"（中间没有内容行）或"标题紧跟在 CLOSE 之后却仍 inside"的可疑点
print("\n可疑模式：OPEN 之后 3 行内又出现标题（说明前一个块没闭合）")
for k in range(len(events) - 1):
    s1, l1, k1, t1 = events[k]
    s2, l2, k2, t2 = events[k + 1]
    if k1 == "OPEN " and k2 == "OPEN ":
        seg = lines[l1: l2 - 1]
        heads = [x for x in seg if re.match(r"^#{2,3} ", x)]
        if heads:
            print("  ⚠️ L%d(OPEN) 与 L%d(OPEN) 之间有标题: %s" % (l1, l2, heads[0][:50]))
            print("      → 说明 L%d 这次 CLOSE 之前缺一个闭合围栏" % l2)
