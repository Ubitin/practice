# -*- coding: utf-8 -*-
# 逐行跟踪围栏状态，找出所有"标题落在代码块内"和"未闭合"的位置
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

inside = False
open_at = 0
fence_total = 0
bad_heads = []
for i, l in enumerate(lines):
    if re.match(r"^\s*```", l):
        fence_total += 1
        if inside:
            inside = False
        else:
            inside = True
            open_at = i + 1
        continue
    if inside and re.match(r"^#{2,3} ", l):
        bad_heads.append((open_at, i + 1, l[:60]))

print("总行数 =", len(lines))
print("围栏行数 =", fence_total, "（偶数 =", fence_total % 2 == 0, "）")
print("最终状态 =", "未闭合 ✘" if inside else "全部闭合 ✅")
if inside:
    print("  未闭合的开启围栏在 L%d" % open_at)
print("标题掉进代码块的次数 =", len(bad_heads))
for a, b, t in bad_heads[:10]:
    print("   L%d 开启的块 → L%d 标题: %s" % (a, b, t))
