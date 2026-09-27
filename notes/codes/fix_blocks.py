# -*- coding: utf-8 -*-
# 修复手册里未闭合的代码块：在它结束处补一个 ```
import io, os, re, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")


def report(tag):
    fences = [i for i, l in enumerate(lines) if re.match(r"^\s*```", l)]
    inside = False
    bad = []
    open_at = 0
    for i, l in enumerate(lines):
        if re.match(r"^\s*```", l):
            inside = not inside
            if inside:
                open_at = i + 1
            continue
        if re.match(r"^#{2,3} ", l) and inside:
            bad.append(open_at)
    print("%s: 行数=%d 围栏=%d 偶数=%s 落在块内的标题=%d 首个出错块起点=%s"
          % (tag, len(lines), len(fences), len(fences) % 2 == 0,
             len(bad), bad[0] if bad else "-"))
    return bad


bad = report("修复前")

# 逐个修复：每次在"出错块"的内容结束处（下一个标题之前）补一个 ```
for _ in range(6):
    if not bad:
        break
    # 重新计算
    inside = False
    open_at = None
    target = None
    for i, l in enumerate(lines):
        if re.match(r"^\s*```", l):
            inside = not inside
            if inside:
                open_at = i + 1
            continue
        if re.match(r"^#{2,3} ", l) and inside:
            target = i          # 标题所在行索引 → 在它前面补闭合
            break
    if target is None:
        break
    # 退到标题前的最后一个非空行之后插入
    k = target - 1
    while k > 0 and lines[k].strip() == "":
        k -= 1
    lines.insert(k + 1, "```")
    print("  已在 L%d 之后补上闭合围栏（该块起始 L%s）" % (k + 1, open_at))

report("修复后")

# 末尾若仍不闭合，也在文件末补一个
fences = [i for i, l in enumerate(lines) if re.match(r"^\s*```", l)]
if len(fences) % 2 == 1:
    lines.append("```")
    print("  文件末尾补齐一个闭合围栏")

report("最终")
io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("已写回", os.path.basename(p))
