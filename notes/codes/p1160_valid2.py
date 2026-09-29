# -*- coding: utf-8 -*-
# 合法生成（保证至少剩 1 人）+ 4 版本对拍
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
VERSIONS = [
    ("惰性删除版（教材提示的做法）", "p1160_lazy.exe"),
    ("真删除+从1开始", "p1160_textbook.exe"),
    ("真删除+从哨兵开始", "p1160_list.exe"),
    ("暴力 std::list（基准）", "p1160_brute.exe"),
]


def gen(seed):
    rng = random.Random(seed)
    n = rng.randint(1, 25)
    alive = [1]
    ins = []
    for i in range(2, n + 1):
        k = rng.choice(alive)          # 参照必须是"还活着"的
        p = rng.choice([0, 1])
        ins.append((k, p))
        alive.append(i)
    # ★ 关键：删除数 m ≤ n-1，保证至少剩 1 人（题面保证有解）
    maxdel = max(0, n - 1)
    m = rng.randint(0, maxdel)
    dels = rng.sample(alive, m) if m else []
    lines = [str(n)] + ["%d %d" % (k, p) for k, p in ins] + [str(m)] + [str(x) for x in dels]
    return "\n".join(lines) + "\n"


def run(exe, inp):
    r = subprocess.run([os.path.join(T, exe)], input=inp, capture_output=True, text=True, timeout=10)
    return r.stdout.split(), r.returncode


print("%-30s %-14s %s" % ("版本", "不一致/1000", "结论"))
print("-" * 60)
for name, exe in VERSIONS:
    if not os.path.exists(os.path.join(T, exe)):
        print("%-30s 未编译" % name); continue
    bad = 0; first = None
    for seed in range(1000):
        inp = gen(seed)
        o, c = run(exe, inp)
        ob, cb = run("p1160_brute.exe", inp)
        if o != ob or c != 0 or cb != 0:
            bad += 1
            if first is None:
                first = (seed, inp, o, ob, c)
    mark = "OK" if bad == 0 else "FAIL"
    print("%-30s %-14s %s" % (name, "%d" % bad, mark))
    if first:
        seed, inp, o, ob, c = first
        print("      seed=%d 退出码=%d" % (seed, c))
        print("      输入前 3 行: %s" % inp.split("\n")[:3])
        print("      该版=%s" % (o[:10] if o else "(空)"))
        print("      暴力=%s" % (ob[:10] if ob else "(空)"))
