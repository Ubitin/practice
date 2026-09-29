# -*- coding: utf-8 -*-
# 体检：后缀表达式讲义（用 glob 定位，避免中文路径编码问题）
import io, glob, os

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
cands = [f for f in glob.glob(os.path.join(d, "*.md")) if "后缀表达式" in os.path.basename(f)]
assert cands, "未找到讲义文件"
p = cands[0]
print("文件 =", os.path.basename(p), os.path.getsize(p), "bytes")
lines = io.open(p, "r", encoding="utf-8").read().split("\n")
fences = [i for i, l in enumerate(lines) if l.lstrip().startswith("```")]
print("行数 =", len(lines), " 围栏 =", len(fences), " 偶数 =", len(fences) % 2 == 0)
# 标题层级
heads = [l for l in lines if l.startswith("## ") or l.startswith("### ")]
print("章节数 =", len(heads))
for h in heads[-8:]:
    print("   ", h[:50])
