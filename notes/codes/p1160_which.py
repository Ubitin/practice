# -*- coding: utf-8 -*-
# 三种"起点选择"的对比实验，找出哪一种与 std::list 暴力版一致
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
B = os.path.join(T, "p1160_brute.exe")


def gen(seed):
    rng = random.Random(seed)
    n = rng.randint(1, 30)
    lines = [str(n)]
    for i in range(2, n + 1):
        lines.append("%d %d" % (rng.randint(1, i - 1), rng.choice([0, 1])))
    m = rng.randint(0, n)
    lines.append(str(m))
    lines += [str(rng.randint(1, n)) for _ in range(m)]
    return "\n".join(lines) + "\n"


def run(exe, inp):
    r = subprocess.run([exe], input=inp, capture_output=True, text=True, timeout=10)
    return r.stdout.split(), r.returncode


res = {}
for name, exe in [("教材版(从1开始)", "p1160_textbook.exe"),
                  ("漏行版(从哨兵开始)", "p1160_list.exe"),
                  ("补行版(从哨兵开始)", "p1160_fixed.exe")]:
    exe = os.path.join(T, exe)
    if not os.path.exists(exe):
        print(name, "→ 未编译，跳过"); continue
    bad = 0; first = None
    for seed in range(800):
        inp = gen(seed)
        o, c = run(exe, inp)
        ob, cb = run(B, inp)
        if o != ob or c != 0:
            bad += 1
            if first is None:
                first = (seed, o, ob)
    res[name] = (bad, first)
    print("%-22s 不一致 %3d / 800   %s" % (name, bad, "OK" if bad == 0 else "FAIL"))
    if first:
        print("     首个错例 seed=%d  该版输出=%s  暴力版=%s" % (first[0], first[1][:8], first[2][:8]))
