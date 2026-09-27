# -*- coding: utf-8 -*-
# 最终体检：讲义/索引/手册/docx 四件套一致性
import io, os, glob

D = r"D:\lenovo\chat_with_deepseek_harness"
d = os.path.join(D, "讲义")

md = glob.glob(os.path.join(d, "*.md"))
expect = len(md) - 2                     # 排除索引与手册
idx = io.open(os.path.join(d, "00-知识索引与复习路线图.md"), "r", encoding="utf-8").read().split("\n")
hb = io.open(os.path.join(d, "机试代码模板速查手册.md"), "r", encoding="utf-8").read().split("\n")

declared = int(__import__("re").search(r"收录：\*\*(\d+) 份\*\*", idx[4]).group(1))
print("【1】讲义文件 =", len(md), "  应收录 =", expect, "  索引声明 =", declared,
      "✅" if expect == declared else "✘")

# 索引表格块列数
from collections import defaultdict
cur = None
blocks = []
for j, l in enumerate(idx):
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
print("【2】索引表格块 =", len(blocks), " 列数不齐 =", len(bad), "✅" if not bad else "✘")

# 日志是否都在日志区内
log_start = next(i for i, l in enumerate(idx) if l.startswith("## 六、更新日志"))
mis = [i + 1 for i, l in enumerate(idx) if l.startswith("| 2026-") and i < log_start]
print("【3】错位日志行 =", len(mis), "✅" if not mis else "✘ " + str(mis))

# 手册围栏与块内标题
fences = [i for i, l in enumerate(hb) if l.lstrip().startswith("```")]
inside = False
bad_head = 0
for l in hb:
    if l.lstrip().startswith("```"):
        inside = not inside
        continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad_head += 1
print("【4】手册围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0,
      " 块内标题 =", bad_head, "✅" if (len(fences) % 2 == 0 and bad_head == 0) else "✘")

# 手册关键小节是否齐全
keys = ["### 2.2.1 substr 的返回值必须接住", "### 3.15", "### 3.16", "### 3.17",
        "### 6.13 快速幂", "### 6.14 k 的倍数", "### 6.15 全局取 max",
        "### 6.16 组合数", "### 6.17 gcd"]
missing = [k for k in keys if not any(l.startswith(k) for l in hb)]
print("【5】关键小节缺失 =", missing if missing else "无 ✅")

# docx
dx = glob.glob(os.path.join(d, "*.docx"))
print("【6】打印版 docx =", os.path.basename(dx[0]), os.path.getsize(dx[0]), "bytes")

# 本轮补做的文件
print("\n【7】本轮补做的交付物：")
for f in ["讲义\\辗转相除求gcd-那行while在干什么.md"]:
    fp = os.path.join(D, f)
    print("   ", f, "存在 =", os.path.exists(fp), os.path.getsize(fp) if os.path.exists(fp) else "")
