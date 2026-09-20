#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""批量对拍: 参考实现 vs 指定可执行文件(文件重定向,不用管道)。

用法: python cmp01.py <exe路径> [范围上限]
"""
import itertools
import os
import subprocess
import sys
import tempfile


def ref(a, b, c):
    out = []
    for x in range(123, 988):
        if (x * b) % a or (x * c) % a:
            continue
        y, z = x * b // a, x * c // a
        if y > 999 or z > 999:
            continue
        d = f'{x}{y}{z}'
        if len(d) == 9 and set(d) == set('123456789'):
            out.append(f'{x} {y} {z}')
    return '\n'.join(out) if out else 'No!!!'


def run(exe, inp, tmpdir):
    fi = os.path.join(tmpdir, 'in.txt')
    fo = os.path.join(tmpdir, 'out.txt')
    with open(fi, 'w', encoding='ascii') as f:
        f.write(inp + '\n')
    with open(fi, 'rb') as fin, open(fo, 'wb') as fout:
        subprocess.run([exe], stdin=fin, stdout=fout, timeout=20)
    txt = open(fo, encoding='utf-8', errors='replace').read()
    return '\n'.join(l.rstrip() for l in txt.strip().splitlines())


def main():
    exe = sys.argv[1]
    hi = int(sys.argv[2]) if len(sys.argv) > 2 else 12
    cases = [f'{a} {b} {c}' for a, b, c in itertools.combinations(range(1, hi + 1), 3)]
    # 再补一些 A>B 的非法/倒序输入,观察鲁棒性
    cases += ['3 1 2', '9 1 2', '9 2 3', '5 1 4', '7 1 2']
    tmp = os.path.join(os.path.dirname(os.path.abspath(exe)), '_cmp_tmp')
    os.makedirs(tmp, exist_ok=True)
    bad = []
    for cs in cases:
        a, b, c = (int(v) for v in cs.split())
        exp = ref(a, b, c)
        got = run(exe, cs, tmp)
        if exp != got:
            bad.append((cs, exp, got))
    print(f'[cmp] 用例 {len(cases)} 个,不一致 {len(bad)} 个')
    for cs, exp, got in bad[:6]:
        e = exp.splitlines()
        g = got.splitlines()
        print(f'  ✗ [{cs}] 参考 {len(e)} 行 / 你的程序 {len(g)} 行')
        print(f'      参考首行: {e[0] if e else "<空>"}   你的首行: {g[0] if g else "<空>"}')
    if bad:
        no_sol = [cs for cs, e, g in bad if e == 'No!!!']
        print(f'  其中"应为 No!!! 却没输出"的用例共 {len(no_sol)} 个,例如: {", ".join(no_sol[:5])}')


if __name__ == '__main__':
    main()
