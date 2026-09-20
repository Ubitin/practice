#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""P1036 选数 —— 对拍 / 计时工具。

用法:
  python cmp1036.py <exe> sample   官方样例
  python cmp1036.py <exe> random   随机小数据对拍(60 组)
  python cmp1036.py <exe> time     n=20 k=10 大数据计时
  python cmp1036.py <exe> all      全部
"""
import itertools
import os
import random
import subprocess
import sys
import time


def isprime(x):
    if x < 2:
        return False
    if x % 2 == 0:
        return x == 2
    i = 3
    while i * i <= x:
        if x % i == 0:
            return False
        i += 2
    return True


def ref(n, k, a):
    return sum(1 for c in itertools.combinations(a, k) if isprime(sum(c)))


def run(exe, inp, tmp, timeout=60):
    fi = os.path.join(tmp, 'in.txt')
    fo = os.path.join(tmp, 'out.txt')
    with open(fi, 'w', encoding='ascii') as f:
        f.write(inp + '\n')
    t0 = time.time()
    with open(fi, 'rb') as fin, open(fo, 'wb') as fout:
        subprocess.run([exe], stdin=fin, stdout=fout, timeout=timeout)
    dt = time.time() - t0
    return open(fo, encoding='utf-8', errors='replace').read().strip(), dt


def main():
    exe = os.path.abspath(sys.argv[1])
    mode = sys.argv[2] if len(sys.argv) > 2 else 'all'
    tmp = os.path.join(os.path.dirname(exe), '_t1036')
    os.makedirs(tmp, exist_ok=True)

    if mode in ('sample', 'all'):
        inp = '4 3\n3 7 12 19'
        got, _ = run(exe, inp, tmp)
        print(f'[样例] 4 3 / 3 7 12 19 → 你的程序 {got!r}   期望 1   {"OK" if got == "1" else "✗ 错"}')

    if mode in ('random', 'all'):
        rnd = random.Random(20260905)
        bad = 0
        total = 60
        for t in range(total):
            n = rnd.randint(2, 8)
            k = rnd.randint(1, n - 1)
            a = [rnd.choice([1, 1, 2, 3, 4, 5, 7, 12, 19, 23, 30]) for _ in range(n)]
            inp = f'{n} {k}\n' + ' '.join(map(str, a))
            exp = str(ref(n, k, a))
            got, _ = run(exe, inp, tmp)
            if got != exp:
                bad += 1
                if bad <= 3:
                    print(f'  ✗ n={n} k={k} a={a}  期望 {exp}  你的程序 {got}')
        print(f'[对拍] {total} 组随机小数据,错误 {bad} 组')

    if mode in ('time', 'all'):
        rnd = random.Random(7)
        n, k = 20, 10
        a = [rnd.randint(4000000, 5000000) for _ in range(n)]
        inp = f'{n} {k}\n' + ' '.join(map(str, a))
        got, dt = run(exe, inp, tmp, timeout=120)
        exp = str(ref(n, k, a))
        print(f'[计时] n=20 k=10 大数用例: {dt:.2f}s  输出 {got} (期望 {exp})')


if __name__ == '__main__':
    main()
