# -*- coding: utf-8 -*-
# 用与诊断脚本相同的正确逻辑，检查每个 ## / ### 标题是否落在代码块内
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

inside = False
open_at = 0
bad = []
for i, l in enumerate(lines):
    if re.match(r"^\s*```", l):
        inside = not inside
        if inside:
            open_at = i + 1
        continue
    if re.match(r"^#{2,3} ", l) and inside:
        bad.append((open_at, i + 1, l[:50]))

print("总行数 =", len(lines))
print("落在代码块内的标题数 =", len(bad))
for a, b, t in bad:
    print("   L%d 开启 → L%d: %s" % (a, b, t))

# 定向确认几处
for target in ["### 6.15", "### 6.16", "### 6.13", "## 誊抄建议"]:
    for i, l in enumerate(lines):
        if l.startswith(target):
            # 算这一行的块深度
            dep = 0
            for j in range(i):
                if re.match(r"^\s*```", lines[j]):
                    dep = 0 if dep else 1
            print("%-16s 在 L%-5d 深度=%d %s" % (target, i + 1, dep, "✅ 在块外" if dep == 0 else "✘ 在块内"))
            break
