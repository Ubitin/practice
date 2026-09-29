# -*- coding: utf-8 -*-
# 补两行后的版本 vs 参考实现 + 官方样例
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
F = os.path.join(T, "p1160_user_fixed.exe")   # 补两行版
R = os.path.join(T, "p1160_sentinel.exe")     # 参考实现


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


# 官方样例
sample = "4\n1 0\n2 1\n1 0\n2\n3\n"
o, c = run(F, sample)
print("官方样例: 补两行版 = %s   期望 2 4 1   %s" % (" ".join(o), "OK" if o == ["2", "4", "1"] else "FAIL"))

bad = 0; first = None
for seed in range(1000):
    inp = gen(seed)
    o, c = run(F, inp)
    ob, cb = run(R, inp)
    if o != ob or c != 0:
        bad += 1
        if first is None:
            first = (seed, inp, o, ob, c)

print("对拍 1000 组: 不一致 =", bad, "OK" if bad == 0 else "FAIL")
if first:
    seed, inp, o, ob, c = first
    print("  首个错例 seed=%d 你的=%s 参考=%s" % (seed, o[:8], ob[:8]))
