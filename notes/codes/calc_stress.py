# -*- coding: utf-8 -*-
# 用 Python 独立实现同一套后缀表达式求值，与 C++ 修正版对拍
# 语法：数字（十进制整数）后面跟 '.' 表示推入栈；+ - * / 为二元运算；'@' 输出栈顶
import subprocess, os, random

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
EXE = os.path.join(T, "calc_fixed.exe")


def ref_eval(expr):
    """独立的 Python 实现：用一个列表当栈，逐字符扫描"""
    st, num = [], ""
    out = []
    for ch in expr:
        if ch.isdigit():
            num += ch
        elif ch == '.':
            if num != "":
                st.append(int(num)); num = ""
        elif ch in "+-*/":
            x = st.pop(); y = st.pop()
            if ch == '+': st.append(y + x)
            elif ch == '-': st.append(y - x)
            elif ch == '*': st.append(y * x)
            else: st.append(y // x if (y < 0) != (x < 0) and y % x else y // x)
        elif ch == '@':
            out.append(str(st[-1]))
    return out


def run(expr):
    r = subprocess.run([EXE], input=expr, capture_output=True, text=True)
    return r.stdout.split(), r.returncode


cases = ["9.@", "12.@", "123.@", "5.3.+@", "12.34.+@",
         "5.3.-@", "5.3.*@", "6.2./@", "1.2.+3.*@", "100.200.+@"]
random.seed(3)
ops = ["+", "-", "*"]
for _ in range(40):
    a = random.randint(0, 50); b = random.randint(0, 50)
    op = random.choice(ops)
    cases.append("%d.%d.%s@" % (a, b, op))
for _ in range(10):
    a = random.randint(1, 20); b = random.randint(1, 20)
    cases.append("%d.%d./@" % (a * b, b))

bad = 0
first = None
for e in cases:
    got, rc = run(e)
    exp = ref_eval(e)
    if got != exp or rc != 0:
        bad += 1
        if first is None:
            first = (e, got, exp, rc)

print("用例数 =", len(cases))
print("不一致/异常 =", bad, "OK" if bad == 0 else "FAIL")
if first:
    print("  首个问题: 输入 %s  C++输出=%s  Python=%s  退出码=%d" % first)
else:
    print("  抽样:", [(e, run(e)[0]) for e in cases[:6]])
