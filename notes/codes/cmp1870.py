#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CF 1870C Colorful Table —— 样例校验 / 大数据计时。

用法:
  python cmp1870.py <exe> sample
  python cmp1870.py <exe> time [n] [k]
  python cmp1870.py <exe> all
"""
import os
import random
import subprocess
import sys
import time

SAMPLE_IN = """5
2 1
1 1
2 2
1 2
3 5
3 2 4
4 2
1 2 1 2
5 3
1 2 3 2 1
"""

SAMPLE_OUT = """4
4 2
0 6 6 2 0
8 6
10 6 2
"""


def run(exe, inp, tmp, timeout=60):
    fi = os.path.join(tmp, 'in.txt')
    fo = os.path.join(tmp, 'out.txt')
    with open(fi, 'w', encoding='ascii') as f:
        f.write(inp)
    t0 = time.time()
    with open(fi, 'rb') as fin, open(fo, 'wb') as fout:
        try:
            subprocess.run([exe], stdin=fin, stdout=fout, timeout=timeout)
        except subprocess.TimeoutExpired:
            return None, time.time() - t0
    return open(fo, encoding='utf-8', errors='replace').read(), time.time() - t0


def main():
    exe = os.path.abspath(sys.argv[1])
    mode = sys.argv[2] if len(sys.argv) > 2 else 'all'
    tmp = os.path.join(os.path.dirname(exe), '_t1870')
    os.makedirs(tmp, exist_ok=True)

    if mode in ('sample', 'all'):
        got, dt = run(exe, SAMPLE_IN, tmp)
        exp_lines = [l.split() for l in SAMPLE_OUT.strip().splitlines()]
        if got is None:
            print(f'[样例] 超时({dt:.1f}s)')
        else:
            got_lines = [l.split() for l in got.strip().splitlines() if l.strip()]
            ok = got_lines == exp_lines
            print(f'[样例] {"全部通过 ✅" if ok else "有错 ❌"}')
            for i, (e, g) in enumerate(zip(exp_lines, got_lines), 1):
                flag = 'OK ' if e == g else '✗  '
                print(f'  {flag} 第{i}组 期望 {" ".join(e):14s} 你的 {" ".join(g)}')

    if mode == 'ab':
        exe2 = os.path.abspath(sys.argv[3])
        cases = int(sys.argv[4]) if len(sys.argv) > 4 else 300
        rnd = random.Random(42)
        tests = []
        for _ in range(cases):
            n = rnd.randint(1, 12)
            k = rnd.randint(1, 12)
            a = [rnd.randint(1, k) for _ in range(n)]
            tests.append((n, k, a))
        inp = f'{cases}\n' + ''.join(f'{n} {k}\n' + ' '.join(map(str, a)) + '\n'
                                     for n, k, a in tests)
        o1, _ = run(exe, inp, tmp)
        o2, _ = run(exe2, inp, tmp)
        t1, t2 = o1.split(), o2.split()
        if t1 == t2:
            print(f'[交叉验证] {cases} 组随机数据,两个实现输出完全一致 ✅')
        else:
            bad = [(i, x, y) for i, (x, y) in enumerate(zip(t1, t2)) if x != y]
            print(f'[交叉验证] {cases} 组,不一致 {len(bad)} 处,前 3 处: {bad[:3]}')

    if mode in ('time', 'all'):
        n = int(sys.argv[3]) if len(sys.argv) > 3 else 200000
        k = int(sys.argv[4]) if len(sys.argv) > 4 else n
        rnd = random.Random(1)
        a = [rnd.randint(1, k) for _ in range(n)]
        inp = f'1\n{n} {k}\n' + ' '.join(map(str, a)) + '\n'
        got, dt = run(exe, inp, tmp, timeout=15)
        if got is None:
            print(f'[计时] n={n} k={k}: 15 秒内没跑完 ❌ (TLE)')
        else:
            print(f'[计时] n={n} k={k}: {dt:.2f}s,输出 {len(got.split())} 个数')


if __name__ == '__main__':
    main()
