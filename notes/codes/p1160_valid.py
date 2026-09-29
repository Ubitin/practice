# -*- coding: utf-8 -*-
# 按题面语义生成合法数据（插入参照必须是"仍活着"的同学），再对拍 4 个版本
import random, subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
VERSIONS = [
    ("惰性删除版", "p1160_lazy.exe"),
    ("教材风格版(从1开始)", "p1160_textbook.exe"),
    ("漏行版(从哨兵开始)", "p1160_list.exe"),
    ("暴力 std::list", "p1160_brute.exe"),
]


def gen(seed):
    """合法生成：先造 n 个插入操作（参照只能取"当前活着的"），再造删除（只删活着的）"""
    rng = random.Random(seed)
    n = rng.randint(1, 25)
    alive = [1]                      # 队列里活着的编号
    ins = []
    for i in range(2, n + 1):
        k = rng.choice(alive)        # ★ 只用活着的当参照
        p = rng.choice([0, 1])
        ins.append((k, p))
        alive.append(i)
    # 删除：从活着的里面随机挑若干个（保证至少剩 1 个）
    m = rng.randint(0, max(0, len(alive) - 1))
    dels = rng.sample(alive, m) if m else []
    lines = [str(n)]
    lines += ["%d %d" % (k, p) for k, p in ins]
    lines.append(str(len(dels)))
    lines += [str(x) for x in dels]
    return "\n".join(lines) + "\n"


def run(exe, inp):
    r = subprocess.run([os.path.join(T, exe)], input=inp, capture_output=True, text=True, timeout=10)
    return r.stdout.split(), r.returncode


print("%-24s %-12s %s" % ("版本", "不一致/800", "结论"))
print("-" * 52)
for name, exe in VERSIONS:
    if not os.path.exists(os.path.join(T, exe)):
        print("%-24s %s" % (name, "未编译")); continue
    bad = 0; first = None
    for seed in range(800):
        inp = gen(seed)
        o, c = run(exe, inp)
        ob, cb = run("p1160_brute.exe", inp)
        if o != ob or c != 0 or cb != 0:
            bad += 1
            if first is None:
                first = (seed, inp, o, ob, c)
    print("%-24s %-12s %s" % (name, "%d" % bad, "OK" if bad == 0 else "FAIL"))
    if first:
        seed, inp, o, ob, c = first
        print("      seed=%d 退出码=%d" % (seed, c))
        print("      该版=%s" % (o[:10] if o else "(空)"))
        print("      暴力=%s" % (ob[:10] if ob else "(空)"))
