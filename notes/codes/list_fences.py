# -*- coding: utf-8 -*-
# 打印 L820~L870 与 L1635~L1650 的围栏序列，手工核对配对
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")
print("python 读到行数 =", len(lines))

inside = False
para = 0
print("\n--- L640 之前的围栏配对情况 ---")
for i, l in enumerate(lines):
    if re.match(r"^\s*```", l):
        para += 1
        kind = "CLOSE" if inside else "OPEN "
        if i + 1 <= 1670:
            print("  #%-3d L%-5d %s" % (para, i + 1, kind))
        inside = not inside
print("\n最终 inside =", inside, " 围栏总数 =", para)
