#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""检查手册代码行是否超出打印宽度（A4 - 左右各 14mm ≈ 516pt，8pt 等宽字体）。

用法: python check_width.py <手册.md> [字号]
"""
import re
import sys

try:
    import pymupdf as fitz
except ImportError:
    import fitz

FD = r'C:\Windows\Fonts'
_con = fitz.Font(fontfile=FD + r'\consola.ttf')
_ya = fitz.Font(fontfile=FD + r'\msyh.ttc')
FENCE = re.compile(r'^\s*(`{3,}|~{3,})')
LIMIT = 516.0            # A4(595.3pt) - 左右各 14mm(≈39.7pt)


def width(s, size):
    t = 0.0
    for ch in s:
        t += _con.text_length(ch, size) if ch.isascii() else _ya.text_length(ch, size)
    return t


def main():
    path = sys.argv[1]
    size = float(sys.argv[2]) if len(sys.argv) > 2 else 8.0
    lines = open(path, encoding='utf-8').read().split('\n')
    in_fence, rows = False, []
    for no, ln in enumerate(lines, 1):
        if FENCE.match(ln):
            in_fence = not in_fence
            continue
        if in_fence and ln.strip():
            rows.append((width(ln, size), no, ln))
    rows.sort(reverse=True)
    over = [r for r in rows if r[0] > LIMIT]
    print(f'[width] 字号 {size}pt,限宽 {LIMIT:.0f}pt,代码行 {len(rows)} 行,超宽 {len(over)} 行')
    for wd, no, ln in rows[:8]:
        print(f'  {"WRAP" if wd > LIMIT else "OK  "} {wd:6.1f}pt  L{no}: {ln[:72]}')


if __name__ == '__main__':
    main()
