# -*- coding: utf-8 -*-
# 分析不一致的性质：是不是"全被删空"导致的（P1160 保证至少剩一人）
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
A = os.path.join(T, "p1160_list.exe")
B = os.path.join(T, "p1160_brute.exe")


def gen(seed, empty_ok=True):
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
    return r.stdout.split()


cat = {"双方都空": 0, "教材空/暴力非空": 0, "双方非空且不同": 0, "其它": 0}
examples = {}
for seed in range(600):
    inp = gen(seed)
    oa, ob = run(A, inp), run(B, inp)
    if oa == ob:
        continue
    if not oa and ob:
        cat["教材空/暴力非空"] += 1
        examples.setdefault("教材空/暴力非空", (seed, inp, oa, ob))
    elif not oa and not ob:
        cat["双方都空"] += 1
    elif oa and ob:
        cat["双方非空且不同"] += 1
        examples.setdefault("双方非空且不同", (seed, inp, oa, ob))
    else:
        cat["其它"] += 1
        examples.setdefault("其它", (seed, inp, oa, ob))

print("不一致分类统计：")
for k, v in cat.items():
    print("   %-18s : %d" % (k, v))

for k, (seed, inp, oa, ob) in examples.items():
    print("\n【%s】seed=%d" % (k, seed))
    print("   输入头两行:", inp.split("\n")[:2])
    print("   教材版前 12:", oa[:12])
    print("   暴力版前 12:", ob[:12])
