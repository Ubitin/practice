#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""P1618 参考实现 + 找"无解"输入,用于对比 test_/01.cpp 的行为。

参考定义: 枚举最小数 x(123~987),y=x*b/a, z=x*c/a 必须整除、
三者都是三位数,且 9 个数字恰好是 1~9 各一次;无解输出 No!!!。
"""
import itertools
import sys


def ref(a, b, c):
    out = []
    sieve = 0          # 只通过"整除 + y,z<=999"的候选个数(用户程序里 cnt 计的就是它)
    for x in range(123, 988):
        if (x * b) % a or (x * c) % a:
            continue
        y, z = x * b // a, x * c // a
        if y > 999 or z > 999:
            continue
        sieve += 1
        digits = f'{x}{y}{z}'
        if len(digits) == 9 and set(digits) == set('123456789'):
            out.append(f'{x} {y} {z}')
    return out, sieve


def main():
    mode = sys.argv[1] if len(sys.argv) > 1 else 'find'
    if mode == 'run':                      # 单组: 打印参考输出
        a, b, c = (int(v) for v in sys.argv[2:5])
        sol, sieve = ref(a, b, c)
        print('\n'.join(sol) if sol else 'No!!!')
        return
    # 找"无解但 cnt>0"的输入
    print('a  b  c | 参考解数 | cnt(候选数)')
    found = []
    for a, b, c in itertools.combinations(range(1, 31), 3):
        sol, sieve = ref(a, b, c)
        if not sol and sieve > 0:
            found.append((a, b, c, sieve))
            if len(found) <= 8:
                print(f'{a:2d} {b:2d} {c:2d} |    0     |   {sieve}')
    print(f'共找到 {len(found)} 组"无解但候选>0"的输入 (a<b<c<=30)')
    if found:
        print('示例输入:', ' '.join(map(str, found[0][:3])))


if __name__ == '__main__':
    main()
