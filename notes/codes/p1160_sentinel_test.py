# -*- coding: utf-8 -*-
# 最新版（哨兵0 + 惰性删除 + 从 a[0].nxt 走）对拍
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
NEW = os.path.join(T, "p1160_sentinel.exe")
BRU = os.path.join(T, "p1160_brute.exe")


def gen(seed):
    rng = random.Random(seed)
    n = rng.randint(1, 25)
    alive = [1]; ins = []
    for i in range(2, n + 1):
        k = rng.choice(alive); p = rng.choice([0, 1])
        ins.append((k, p)); alive.append(i)
    m = rng.randint(0, max(0, n - 1))          # 至少剩 1 人
    dels = rng.sample(alive, m) if m else []
    return "\n".join([str(n)] + ["%d %d" % t for t in ins] + [str(m)] + [str(x) for x in dels]) + "\n"


def run(exe, inp):
    r = subprocess.run([exe], input=inp, capture_output=True, text=True, timeout=10)
    return r.stdout.split(), r.returncode


bad = 0; first = None
for seed in range(1000):
    inp = gen(seed)
    o, c = run(NEW, inp)
    ob, cb = run(BRU, inp)
    if o != ob or c != 0 or cb != 0:
        bad += 1
        if first is None:
            first = (seed, inp, o, ob, c)

print("哨兵0版 vs 暴力版：1000 组，不一致 =", bad, "OK" if bad == 0 else "FAIL")
if first:
    seed, inp, o, ob, c = first
    print("  seed=%d 退出码=%d" % (seed, c))
    print("  输入前 4 行:", inp.split("\n")[:4])
    print("  该版:", o[:12])
    print("  暴力:", ob[:12])
else:
    for seed in (0, 1, 2):
        inp = gen(seed)
        print("  抽样 seed=%d 输出=%s" % (seed, run(NEW, inp)[0][:12]))
