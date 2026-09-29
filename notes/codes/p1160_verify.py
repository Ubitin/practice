# -*- coding: utf-8 -*-
# 三版对比：漏半句的教材复刻 vs 修正版 vs std::list 暴力版
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
A = os.path.join(T, "p1160_list.exe")     # 漏了 a[le].nxt = rt
C = os.path.join(T, "p1160_fixed.exe")    # 补上
B = os.path.join(T, "p1160_brute.exe")    # std::list


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


badA = badC = 0
firstA = None
for seed in range(800):
    inp = gen(seed)
    oa, ca = run(A, inp)
    oc, cc = run(C, inp)
    ob, cb = run(B, inp)
    if oa != ob or ca != 0:
        badA += 1
        if firstA is None:
            firstA = (seed, inp, oa, ob)
    if oc != ob or cc != 0:
        badC += 1
        if badC == 1:
            print("修正版也错! seed =", seed)
            print(inp)
            print(" 修正版:", oc[:20], " 暴力版:", ob[:20])

print("对拍 800 组（与 std::list 暴力版比）：")
print("   漏了 a[le].nxt = rt 的版本 : 不一致 %d" % badA)
print("   修正版                    : 不一致 %d  %s" % (badC, "OK" if badC == 0 else "FAIL"))
if firstA:
    seed, inp, oa, ob = firstA
    print("\n漏行版首个错例 seed=%d，输入前两行=%s" % (seed, inp.split("\n")[:2]))
    print("   漏行版输出:", oa[:12], "(空)" if not oa else "")
    print("   暴力版输出:", ob[:12])
