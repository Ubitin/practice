# -*- coding: utf-8 -*-
# 01_fraction_fixed2.cpp vs Python fractions.Fraction
import subprocess, random
from fractions import Fraction

EXE = r"D:\lenovo\chat_with_deepseek_harness\test_\zzf_c++14.exe"


def truth(a, b, c, d):
    x, y = Fraction(a, b), Fraction(c, d)

    def s(f):
        return "%d/%d" % (f.numerator, f.denominator)
    return ["(%d/%d)+(%d/%d)=%s" % (a, b, c, d, s(x + y)),
            "(%d/%d)-(%d/%d)=%s" % (a, b, c, d, s(x - y)),
            "(%d/%d)*(%d/%d)=%s" % (a, b, c, d, s(x * y)),
            "(%d/%d)/(%d/%d)=%s" % (a, b, c, d, s(x / y))]


cases = [(1, 2, 1, 3), (1, 3, 1, 2), (0, 5, 1, 2), (2, 4, 3, 9), (5, 1, 1, 5),
         (7, 3, 7, 3), (100, 7, 3, 11), (1, 2, 2, 4), (3, 4, 1, 4), (1, 1, 1, 1),
         (10, 3, 5, 6), (0, 1, 0, 1)]
# 含负数的组合（题面可能给负分母）
for _ in range(30):
    cases.append((random.randint(-20, 20) or 1, random.randint(1, 20),
                  random.randint(-20, 20) or 1, random.randint(1, 20)))
# 带负分母的写法 a/-b（题面罕见，但测一下解析）
neg_den = [(1, -2, 1, 3), (3, -4, -5, 6)]

bad = 0
skipped = 0
first = None
for (a, b, c, d) in cases:
    if c == 0:
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

print("常规用例 =", len(cases), " 跳过(除数分子0) =", skipped, " 不一致 =", bad,
      "✅" if bad == 0 else "✘")
if first:
    print("首个不一致: 输入", first[0])
    for g, e in zip(first[1], first[2]):
        mark = "  " if g == e else "✘ "
        print("   %s程序: %-34s Python: %s" % (mark, g, e))

# 负分母写法的单独说明
print("\n带负分母的输入（题面若允许 a/-b 这种写法）：")
for (a, b, c, d) in neg_den:
    inp = "%d/%d %d/%d\n" % (a, b, c, d)
    r = subprocess.run([EXE], input=inp, capture_output=True, text=True)
    print("  输入 %-12s 输出:" % inp.strip())
    for l in r.stdout.strip().split("\n"):
        print("     ", l)
