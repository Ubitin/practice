# -*- coding: utf-8 -*-
# P1160 对拍：教材版（数组双向链表+indexx） vs std::list 暴力版
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
A = os.path.join(T, "p1160_list.exe")
B = os.path.join(T, "p1160_brute.exe")


def gen(seed):
    rng = random.Random(seed)
    n = rng.randint(1, 30)
    lines = [str(n)]
    for i in range(2, n + 1):
        k = rng.randint(1, i - 1)
        p = rng.choice([0, 1])
        lines.append("%d %d" % (k, p))
    m = rng.randint(0, n)
    xs = [rng.randint(1, n) for _ in range(m)]
    lines.append(str(m))
    lines += [str(x) for x in xs]
    return "\n".join(lines) + "\n"


bad = 0
first = None
for seed in range(600):
    inp = gen(seed)
    ra = subprocess.run([A], input=inp, capture_output=True, text=True, timeout=10)
    rb = subprocess.run([B], input=inp, capture_output=True, text=True, timeout=10)
    oa = ra.stdout.split()
    ob = rb.stdout.split()
    if oa != ob or ra.returncode != 0 or rb.returncode != 0:
        bad += 1
        if first is None:
            first = (seed, inp, oa, ob, ra.returncode, rb.returncode)

print("对拍 600 组：不一致 =", bad, "OK" if bad == 0 else "FAIL")
if first:
    seed, inp, oa, ob, ca, cb = first
    print("  首个不一致 seed =", seed, " 教材版退出码=%d 暴力版=%d" % (ca, cb))
    print("  输入:\n" + inp)
    print("  教材版:", oa[:20])
    print("  暴力版:", ob[:20])
else:
    # 抽样展示
    for seed in (0, 1, 2):
        inp = gen(seed)
        ra = subprocess.run([A], input=inp, capture_output=True, text=True)
        print("  seed=%d 输入首行=%s 输出=%s" % (seed, inp.split("\n")[0], ra.stdout.strip()[:60]))
