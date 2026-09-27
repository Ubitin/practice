# -*- coding: utf-8 -*-
# 找出最早的"围栏配错"位置：逐块打印每个块的起止行与块内是否含标题
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

fences = [i for i, l in enumerate(lines) if re.match(r"^\s*```", l)]
print("围栏总数 =", len(fences))
print("\n逐块检查（起→止，块内标题数，首行内容）：")
for k in range(0, len(fences) - 1, 2):
    a, b = fences[k], fences[k + 1]
    heads = [j + 1 for j in range(a + 1, b) if re.match(r"^#{2,3} ", lines[j])]
    first = lines[a + 1][:46] if a + 1 < len(lines) else ""
    flag = "  ⚠️ 块内有标题: L%s" % heads if heads else ""
    if heads or k < 6 or (1550 <= a <= 1700):
        print("  块%2d: L%-5d → L%-5d  标题%d  %s%s" % (k // 2 + 1, a + 1, b + 1, len(heads), first, flag))
