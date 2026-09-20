#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""P1164 小A点菜 —— 样例 / 暴力对拍 / 计时。

用法:
  python cmp1164.py <exe> sample
  python cmp1164.py <exe> brute [组数]
  python cmp1164.py <exe> time [n] [m]
  python cmp1164.py <exe> all
"""
import itertools
import os
import random
import subprocess
import sys
import time

SAMPLE_IN = "4 4\n1 1 2 2\n"
SAMPLE_OUT = "3"


def run(exe, inp, tmp, timeout=5):
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
    return open(fo, encoding='utf-8', errors='replace').read().strip(), time.time() - t0


def brute(n, m, a):
    c = 0
    for r in range(n + 1):
        for comb in itertools.combinations(a, r):
            if sum(comb) == m:
                c += 1
    return c


def main():
    exe = os.path.abspath(sys.argv[1])
    mode = sys.argv[2] if len(sys.argv) > 2 else 'all'
    tmp = os.path.join(os.path.dirname(exe), '_t1164')
    os.makedirs(tmp, exist_ok=True)

    if mode in ('sample', 'all'):
        got, dt = run(exe, SAMPLE_IN, tmp, timeout=5)
        if got is None:
            print(f'[样例] 5 秒内没结束 ❌ (超时/死循环, {dt:.1f}s)   期望 {SAMPLE_OUT}')
        else:
            print(f'[样例] 4 4 / 1 1 2 2 → {got!r}   期望 {SAMPLE_OUT}   '
                  f'{"OK" if got == SAMPLE_OUT else "✗ 错"}   ({dt:.2f}s)')

    if mode in ('brute', 'all'):
        cases = int(sys.argv[3]) if len(sys.argv) > 3 else 40
        rnd = random.Random(1164)
        bad = 0
        for t in range(cases):
            n = rnd.randint(1, 7)
            a = [rnd.randint(1, 6) for _ in range(n)]
            m = rnd.randint(1, 12)
            inp = f'{n} {m}\n' + ' '.join(map(str, a)) + '\n'
            exp = str(brute(n, m, a))
            got, _ = run(exe, inp, tmp, timeout=5)
            if got is None:
                bad += 1
                if bad <= 3:
                    print(f'  ✗ n={n} m={m} a={a} → 超时')
            elif got != exp:
                bad += 1
                if bad <= 3:
                    print(f'  ✗ n={n} m={m} a={a}  期望 {exp}  你的 {got}')
        print(f'[对拍] {cases} 组小数据(暴力枚举子集),错误 {bad} 组')

    if mode == 'big':
        # 大数据 + 用 Python 独立跑一遍 DP 交叉验证（暴力枚举在这里不可能）
        n, m = 100, 5000
        a = list(range(1, n + 1))                      # 1..100，凑 5000 ≈ 缺 50 的子集数
        inp = f'{n} {m}\n' + ' '.join(map(str, a)) + '\n'
        py = [0] * (m + 1)
        py[0] = 1
        for x in a:
            for j in range(m, x - 1, -1):
                py[j] += py[j - x]
        got, dt = run(exe, inp, tmp, timeout=10)
        print(f'[大数据] n={n} m={m}(a=1..100): 你的 {got}  Python 参考 {py[m]}  '
              f'{"一致 ✅" if got == str(py[m]) else "不一致 ❌"}  耗时 {dt:.2f}s')

    if mode in ('time', 'all'):
        n = int(sys.argv[3]) if len(sys.argv) > 3 else 100
        m = int(sys.argv[4]) if len(sys.argv) > 4 else 10000
        rnd = random.Random(7)
        a = [rnd.randint(1, 1000) for _ in range(n)]
        inp = f'{n} {m}\n' + ' '.join(map(str, a)) + '\n'
        got, dt = run(exe, inp, tmp, timeout=10)
        if got is None:
            print(f'[计时] n={n} m={m}: 10 秒内没结束 ❌')
        else:
            print(f'[计时] n={n} m={m}: {dt:.2f}s,答案 {got}')


if __name__ == '__main__':
    main()
