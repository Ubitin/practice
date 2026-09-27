# -*- coding: utf-8 -*-
# 分数修正版 vs Python fractions.Fraction 对拍
import subprocess, random
from fractions import Fraction

EXE = r"D:\lenovo\chat_with_deepseek_harness\test_\p01_frac_fixed.exe"


def truth(a, b, c, d):
    x, y = Fraction(a, b), Fraction(c, d)
    def s(f):
        return "%d/%d" % (f.numerator, f.denominator)
    return ["(%d/%d)+(%d/%d)=%s" % (a, b, c, d, s(x + y)),
            "(%d/%d)-(%d/%d)=%s" % (a, b, c, d, s(x - y)),
            "(%d/%d)*(%d/%d)=%s" % (a, b, c, d, s(x * y)),
            "(%d/%d)/(%d/%d)=%s" % (a, b, c, d, s(x / y))]


cases = [(1, 2, 1, 3), (1, 3, 1, 2), (0, 5, 1, 2), (2, 4, 3, 9), (-1, 2, 1, 3),
         (5, 1, 1, 5), (7, 3, 7, 3), (1, 1, 1, 1), (-3, 4, -5, 6), (100, 7, 3, 11)]
random.seed(1)
for _ in range(40):
    cases.append((random.randint(-20, 20), random.randint(1, 20),
                  random.randint(-20, 20), random.randint(1, 20)))

bad = 0
first = None
skipped = 0
for (a, b, c, d) in cases:
    if c == 0:                       # 除数分子为 0 ⇒ Python 也会抛 ZeroDivisionError，跳过
        skipped += 1
        continue
    inp = "%d/%d %d/%d\n" % (a, b, c, d)
    r = subprocess.run([EXE], input=inp, capture_output=True, text=True)
    got = [l.strip() for l in r.stdout.strip().split("\n") if l.strip()]
    exp = truth(a, b, c, d)
    if got != exp:
        bad += 1
        if first is None:
            first = (inp.strip(), got, exp)

print("用例数 =", len(cases), " 跳过(除数为0) =", skipped)
print("不一致 =", bad, "✅" if bad == 0 else "✘")
if first:
    print("首个不一致: 输入", first[0])
    print("  程序输出:", first[1])
    print("  Python  :", first[2])
