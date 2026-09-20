#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CF 1805C Place for a Selfie —— 带判题器的对拍 + 计时。

用法:
  python cmp1805c.py <exeA> <exeB> diff [组数]   # 两个实现互相对拍 + 独立合法性校验
  python cmp1805c.py <exe> time [n] [m]          # 计时（最坏情形：全部 NO）
说明: YES 时可以输出任意合法斜率 k，所以不能逐字比较——
      本工具校验：① 两实现 YES/NO 判定一致；② 每个 YES 的 k 确实在输入里且满足 (b-k)^2 < 4ac；
      ③ 每个 NO 必须真的找不到 k（暴力验证）。
"""
import os
import random
import subprocess
import sys
import time


def run(exe, inp, tmp, timeout=20):
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
    return open(fo, encoding='utf-8', errors='replace').read().split(), time.time() - t0


def gen_case(rnd, n_max=8, m_max=6, kmax=20, abcmax=30):
    n = rnd.randint(1, n_max)
    m = rnd.randint(1, m_max)
    ks = sorted(rnd.randint(-kmax, kmax) for _ in range(n))
    paras = [(rnd.randint(1, abcmax), rnd.randint(-abcmax, abcmax), rnd.randint(1, abcmax))
             for _ in range(m)]
    return n, m, ks, paras


def check(tokens, ks, paras):
    """返回 (是否合法, 判定列表)。tokens 是该测试用例的输出片段。"""
    verdicts = []
    i = 0
    for (a, b, c) in paras:
        if i >= len(tokens):
            return False, verdicts
        v = tokens[i]
        if v in ('YES', 'yes'):
            i += 1
            if i >= len(tokens):
                return False, verdicts
            k = int(tokens[i])
            i += 1
            if k not in ks:
                return False, verdicts            # 输出的 k 不在输入里
            if (b - k) ** 2 >= 4 * a * c:
                return False, verdicts            # 不满足无交点条件
            verdicts.append(True)
        elif v in ('NO', 'no'):
            i += 1
            if any((b - k) ** 2 < 4 * a * c for k in ks):
                return False, verdicts            # 明明有解却输出 NO
            verdicts.append(False)
        else:
            return False, verdicts
    return True, verdicts


def main():
    exeA = os.path.abspath(sys.argv[1])
    mode = sys.argv[2]
    tmp = os.path.join(os.path.dirname(exeA), '_t1805c')
    os.makedirs(tmp, exist_ok=True)

    if mode == 'diff':
        exeB = os.path.abspath(sys.argv[3])
        cases = int(sys.argv[4]) if len(sys.argv) > 4 else 200
        rnd = random.Random(1805)
        bad_valid = bad_agree = 0
        for t in range(cases):
            n, m, ks, paras = gen_case(rnd)
            inp = f'1\n{n} {m}\n' + ' '.join(map(str, ks)) + '\n' + \
                  ''.join(f'{a} {b} {c}\n' for a, b, c in paras)
            oA, _ = run(exeA, inp, tmp, timeout=10)
            oB, _ = run(exeB, inp, tmp, timeout=10)
            if oA is None or oB is None:
                bad_valid += 1
                continue
            okA, vA = check(oA, ks, paras)
            okB, vB = check(oB, ks, paras)
            if not okA or not okB:
                bad_valid += 1
                if bad_valid <= 3:
                    print(f'  ✗ 合法性失败 n={n} ks={ks} paras={paras}\n    A={oA} B={oB}')
            elif vA != vB:
                bad_agree += 1
                if bad_agree <= 3:
                    print(f'  ✗ 判定不一致 n={n} ks={ks} paras={paras}\n    A={vA} B={vB}')
        print(f'[对拍] {cases} 组: 合法性错误 {bad_valid} 组, 判定不一致 {bad_agree} 组')

    if mode == 'time':
        n = int(sys.argv[3]) if len(sys.argv) > 3 else 100000
        m = int(sys.argv[4]) if len(sys.argv) > 4 else 100000
        rnd = random.Random(7)
        ks = sorted(rnd.randint(-10**8, 10**8) for _ in range(n))
        # 让所有抛物线都无解：b 远离所有 k，且 4ac 很小
        paras = [(1, 10**8, 1) for _ in range(m)]
        inp = f'1\n{n} {m}\n' + ' '.join(map(str, ks)) + '\n' + \
              ''.join(f'{a} {b} {c}\n' for a, b, c in paras)
        out, dt = run(exeA, inp, tmp, timeout=25)
        if out is None:
            print(f'[计时] n={n} m={m}: 25 秒内没跑完 ❌ (TLE)')
        else:
            print(f'[计时] n={n} m={m}: {dt:.2f}s,输出 {len(out)} 个 token')


if __name__ == '__main__':
    main()
