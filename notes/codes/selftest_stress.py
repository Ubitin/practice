# -*- coding: utf-8 -*-
# 自测参考题 1（等值对计数）暴力对拍
import random, subprocess, itertools

EXE = r"D:\lenovo\chat_with_deepseek_harness\test_\selftest2.exe"


def brute(a):
    c = 0
    for i in range(len(a)):
        for j in range(i + 1, len(a)):
            if a[i] + a[j] == 0:
                c += 1
    return c


random.seed(7)
bad = 0
first = None
for t in range(500):
    n = random.randint(1, 8)
    a = [random.randint(-3, 3) for _ in range(n)]
    inp = "%d\n%s\n" % (n, " ".join(map(str, a)))
    r = subprocess.run([EXE, "1"], input=inp, capture_output=True, text=True)
    got = r.stdout.strip()
    exp = str(brute(a))
    if got != exp:
        bad += 1
        if first is None:
            first = (a, got, exp)

print("对拍 500 组（含 0 重复）：不一致 =", bad, "✅" if bad == 0 else "✘")
if first:
    print("  首个错例: a =", first[0], " 程序 =", first[1], " 暴力 =", first[2])
