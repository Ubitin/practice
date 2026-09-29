# -*- coding: utf-8 -*-
# 对拍：用户的 01.cpp vs 已验证的参考实现（哨兵版）
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
U = os.path.join(T, "p1160_user.exe")      # 用户代码
R = os.path.join(T, "p1160_sentinel.exe")  # 参考实现（1000 组已验证）


def gen(seed):
    rng = random.Random(seed)
    n = rng.randint(1, 20)
    alive = [1]; ins = []
    for i in range(2, n + 1):
        k = rng.choice(alive); p = rng.choice([0, 1])
        ins.append((k, p)); alive.append(i)
    m = rng.randint(0, max(0, n - 1))
    dels = rng.sample(alive, m) if m else []
    return "\n".join([str(n)] + ["%d %d" % t for t in ins] + [str(m)] + [str(x) for x in dels]) + "\n"


def run(exe, inp):
    r = subprocess.run([exe], input=inp, capture_output=True, text=True, timeout=10)
    return r.stdout.split(), r.returncode


bad = 0; first = None
for seed in range(500):
    inp = gen(seed)
    o, c = run(U, inp)
    ob, cb = run(R, inp)
    if o != ob or c != 0:
        bad += 1
        if first is None:
            first = (seed, inp, o, ob, c)

print("你的版本 vs 参考实现：500 组，不一致 =", bad, "OK" if bad == 0 else "FAIL")
if first:
    seed, inp, o, ob, c = first
    print("\n首个错例 seed=%d（退出码=%d）" % (seed, c))
    print("输入:")
    for line in inp.strip().split("\n"):
        print("   ", line)
    print("你的输出:", " ".join(o) if o else "(空)")
    print("参考输出:", " ".join(ob) if ob else "(空)")
