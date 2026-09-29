# -*- coding: utf-8 -*-
# 手册插入 §3.18（内容从文件读，避免引号问题）
import io, os, glob

D = r"D:\lenovo\chat_with_deepseek_harness"
d = os.path.join(D, "讲义")
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")
blk = io.open(os.path.join(D, "test_", "sec_318.txt"), "r", encoding="utf-8").read().rstrip("\n").split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("## 4. 搜索模板"))
print("anchor L%d: %s" % (i + 1, lines[i]))
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

fences = [j for j, l in enumerate(lines) if l.lstrip().startswith("```")]
inside = False
bad = 0
for l in lines:
    if l.lstrip().startswith("```"):
        inside = not inside
        continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad += 1
print("行数 =", len(lines), " 围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0, " 块内标题 =", bad)
