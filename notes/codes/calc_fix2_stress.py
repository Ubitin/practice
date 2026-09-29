# -*- coding: utf-8 -*-
# 修正版v2 vs Python 独立实现对拍（含 EOF/空格/换行/多位数）
import subprocess, os, random

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
NEW = os.path.join(T, "calc_fixed2.exe")


def ref(expr):
    """独立实现：栈 + 字符串累积数字；'@' 结束并输出栈顶"""
    st, num, has = [], "", False
    out = []
    for ch in expr:
        if ch.isdigit():
            num += ch; has = True
        elif ch == '.':
            st.append(int(num) if num else 0); num = ""; has = False
        elif ch in "+-*/":
            if len(st) < 2: continue
            x = st.pop(); y = st.pop()
            if ch == '+': st.append(y + x)
            elif ch == '-': st.append(y - x)
            elif ch == '*': st.append(y * x)
            else: st.append(0 if x == 0 else int(y / x) if (y < 0) != (x < 0) and y % x else y // x)
        elif ch == '@':
            break
    if st: out.append(str(st[-1]))
    elif has: out.append(num)
    return out


def gen(depth=0, rng=None):
    """随机生成后缀表达式"""
    if depth >= 3 or rng.random() < 0.4:
        return "%d." % rng.randint(0, 99)
    left = gen(depth + 1, rng)
    right = gen(depth + 1, rng)
    op = rng.choice("+-*")          # 避开除法（零除两边语义不同）
    return left + right + op


random.seed(11)
cases = []
for _ in range(80):
    cases.append(gen(0, random))
cases += ["9.@", "12.@", "123.@", "1.2.3.++@", "9.@\n", " 9. @ "]

bad = 0
first = None
for e in cases:
    expr = e if e.endswith("@") else e + "@"
    r = subprocess.run([NEW], input=expr, capture_output=True, text=True, timeout=10)
    got = r.stdout.split()
    exp = ref(expr)
    if got != exp or r.returncode != 0:
        bad += 1
        if first is None:
            first = (expr, got, exp, r.returncode)

print("对拍用例 =", len(cases))
print("不一致/异常 =", bad, "OK" if bad == 0 else "FAIL")
if first:
    print("  首个问题:", first)
else:
    print("  抽样:", [(c, subprocess.run([NEW], input=c if c.endswith('@') else c+'@',
                                          capture_output=True, text=True).stdout.strip()) for c in cases[:8]])
