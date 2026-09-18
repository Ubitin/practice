# -*- coding: utf-8 -*-
"""
卡特兰数讲义配图生成器（矢量 SVG，无第三方依赖）

用法:
    python make_figures.py

产出（与本脚本同目录）:
    fig1-两种读法.svg / .png
    fig2-反射原理.svg  / .png
    fig3-首次返回分解.svg / .png
    fig4-n3的五条路径.svg / .png
    _render_figN.html      —— 供 Edge 无头模式渲染 PNG 的包装页

PNG 由 render_figures.ps1 调用 Edge 生成（2 倍高清）。
"""
import os
import urllib.parse

HERE = os.path.dirname(os.path.abspath(__file__))

FONT = "Microsoft YaHei, Segoe UI, Arial, sans-serif"

INK    = '#0f172a'
MUTED  = '#64748b'
GRID   = '#dbe3ec'
AXIS   = '#475569'
DIAG   = '#94a3b8'
CONST  = '#f59e0b'
BLUE   = '#2563eb'
GREEN  = '#16a34a'
RED    = '#dc2626'
VIOLET = '#7c3aed'
PANEL  = '#f8fafc'


# ---------------------------------------------------------------- 基础工具
def esc(s):
    return s.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')


def rich(s):
    """把 'C_{n}' / 'x^{2}' 拆成 [(文本, 类型)]，类型为 n / sub / sup。"""
    out, i, n = [], 0, len(s)
    while i < n:
        if s[i] in '_^' and i + 1 < n and s[i + 1] == '{':
            j = s.index('}', i + 2)
            out.append((s[i + 2:j], 'sub' if s[i] == '_' else 'sup'))
            i = j + 1
        else:
            j = i
            while j < n and not (s[j] in '_^' and j + 1 < n and s[j + 1] == '{'):
                j += 1
            out.append((s[i:j], 'n'))
            i = j
    return out


def text(x, y, s, size=15, fill=INK, anchor='start', weight='400'):
    b = ['<text x="%.1f" y="%.1f" font-family="%s" font-size="%.1f" fill="%s" '
         'text-anchor="%s" font-weight="%s">' % (x, y, FONT, size, fill, anchor, weight)]
    for t, k in rich(s):
        t = esc(t)
        if k == 'sub':
            b.append('<tspan font-size="%.1f" dy="3.2">%s</tspan>'
                     '<tspan font-size="%.1f" dy="-3.2">\u200b</tspan>' % (size * 0.7, t, size))
        elif k == 'sup':
            b.append('<tspan font-size="%.1f" dy="-5">%s</tspan>'
                     '<tspan font-size="%.1f" dy="5">\u200b</tspan>' % (size * 0.7, t, size))
        else:
            b.append('<tspan>%s</tspan>' % t)
    b.append('</text>')
    return ''.join(b)


def line(x1, y1, x2, y2, stroke, w=1.0, dash=None, marker=None, opacity=1.0, cap='butt'):
    a = ' stroke-dasharray="%s"' % dash if dash else ''
    m = ' marker-end="url(#%s)"' % marker if marker else ''
    c = ' stroke-linecap="round"' if cap == 'round' else ''
    return ('<line x1="%.1f" y1="%.1f" x2="%.1f" y2="%.1f" stroke="%s" stroke-width="%.1f"%s%s%s '
            'opacity="%.2f"/>' % (x1, y1, x2, y2, stroke, w, a, m, c, opacity))


def rect(x, y, w, h, fill='none', stroke='none', sw=1.0, dash=None, rx=0, opacity=1.0):
    a = ' stroke-dasharray="%s"' % dash if dash else ''
    return ('<rect x="%.1f" y="%.1f" width="%.1f" height="%.1f" rx="%.1f" fill="%s" stroke="%s" '
            'stroke-width="%.1f"%s opacity="%.2f"/>' % (x, y, w, h, rx, fill, stroke, sw, a, opacity))


def circle(cx, cy, r, fill, stroke='none', sw=1.0, opacity=1.0):
    return ('<circle cx="%.1f" cy="%.1f" r="%.1f" fill="%s" stroke="%s" stroke-width="%.1f" '
            'opacity="%.2f"/>' % (cx, cy, r, fill, stroke, sw, opacity))


def poly(pts, stroke, w=2.8, dash=None, opacity=1.0):
    d = ' '.join('%.1f,%.1f' % p for p in pts)
    a = ' stroke-dasharray="%s"' % dash if dash else ''
    return ('<polyline points="%s" fill="none" stroke="%s" stroke-width="%.1f" stroke-linejoin="round" '
            'stroke-linecap="round"%s opacity="%.2f"/>' % (d, stroke, w, a, opacity))


def arrow(x1, y1, x2, y2, stroke=AXIS, w=1.6, dash=None, marker='arw'):
    return line(x1, y1, x2, y2, stroke, w, dash=dash, marker=marker, cap='round')


def curve(x1, y1, cx, cy, x2, y2, stroke=AXIS, w=1.4, dash=None, marker='arw'):
    a = ' stroke-dasharray="%s"' % dash if dash else ''
    m = ' marker-end="url(#%s)"' % marker if marker else ''
    return ('<path d="M %.1f,%.1f Q %.1f,%.1f %.1f,%.1f" fill="none" stroke="%s" stroke-width="%.1f" '
            'stroke-linecap="round"%s%s/>' % (x1, y1, cx, cy, x2, y2, stroke, w, a, m))


DEFS = ('<defs>'
        '<marker id="arw" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6.5" markerHeight="6.5" '
        'orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="%s"/></marker>'
        '<marker id="arwR" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6.5" markerHeight="6.5" '
        'orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="%s"/></marker>'
        '<marker id="arwG" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="6.5" markerHeight="6.5" '
        'orient="auto-start-reverse"><path d="M0,0 L10,5 L0,10 z" fill="%s"/></marker>'
        '</defs>' % (AXIS, RED, GREEN))


class Canvas(object):
    def __init__(self, w, h):
        self.w, self.h = w, h
        self.parts = []

    def add(self, *items):
        self.parts.extend(items)

    def svg(self):
        body = '\n'.join(self.parts)
        return ('<svg xmlns="http://www.w3.org/2000/svg" width="%d" height="%d" viewBox="0 0 %d %d">'
                '\n<rect width="%d" height="%d" fill="#ffffff"/>\n%s\n%s\n</svg>\n'
                % (self.w, self.h, self.w, self.h, self.w, self.h, DEFS, body))

    def save(self, name):
        p = os.path.join(HERE, name + '.svg')
        with open(p, 'w', encoding='utf-8') as f:
            f.write(self.svg())
        return name + '.svg'


class Grid(object):
    """格点坐标系：格点 (x, y) -> 像素。x 可为负（图 2 需要 -1）。"""

    def __init__(self, ox, oy, cell):
        self.ox, self.oy, self.cell = ox, oy, cell

    def p(self, x, y):
        return (self.ox + x * self.cell, self.oy - y * self.cell)

    def pts(self, ps):
        return [self.p(*q) for q in ps]


def draw_grid(c, g, x0, x1, y0, y1):
    for x in range(x0, x1 + 1):
        p1, p2 = g.p(x, y0), g.p(x, y1)
        c.add(line(p1[0], p1[1], p2[0], p2[1], GRID, 1.0))
    for y in range(y0, y1 + 1):
        p1, p2 = g.p(x0, y), g.p(x1, y)
        c.add(line(p1[0], p1[1], p2[0], p2[1], GRID, 1.0))
    # 坐标轴加粗
    a1, a2 = g.p(x0, 0), g.p(x1, 0)
    c.add(line(a1[0], a1[1], a2[0], a2[1], AXIS, 1.6))
    b1, b2 = g.p(0, y0), g.p(0, y1)
    c.add(line(b1[0], b1[1], b2[0], b2[1], AXIS, 1.6))


def draw_ticks(c, g, x0, x1, y0, y1):
    for x in range(x0, x1 + 1):
        px, py = g.p(x, 0)
        c.add(text(px, py + 20, str(x), 12.5, MUTED, 'middle'))
    for y in range(y0, y1 + 1):
        px, py = g.p(0, y)
        c.add(text(px - 12, py + 4.5, str(y), 12.5, MUTED, 'end'))


def diagonal(c, g, lo, hi, color=DIAG, w=1.8, dash='7 6'):
    p1, p2 = g.p(lo, lo), g.p(hi, hi)
    c.add(line(p1[0], p1[1], p2[0], p2[1], color, w, dash=dash, cap='round'))


def dot(c, g, x, y, color, r=5.2, ring=None):
    px, py = g.p(x, y)
    if ring:
        c.add(circle(px, py, r + 3.5, ring, opacity=0.9))
    c.add(circle(px, py, r, color, stroke='#ffffff', sw=1.6))


def bg_text(c, x, y, w, h, s, size=14.5, fill=INK, anchor='start', weight='400'):
    """先铺一层白色底，再写字：避免文字被网格线穿过。"""
    x0 = x if anchor == 'start' else (x - w if anchor == 'end' else x - w / 2.0)
    c.add(rect(x0, y - h + 4, w, h, '#ffffff', rx=3))
    c.add(text(x, y, s, size, fill, anchor, weight))


# ---------------------------------------------------------------- 图 1
def fig1():
    W, H, CELL = 940, 566, 62
    c = Canvas(W, H)
    c.add(text(28, 42, '卡特兰数几何模型：两种读法（n = 4）', 22, INK, weight='700'))
    c.add(text(28, 70, '每步只能向右 E 或向上 N；起点 (0,0)、终点 (n,n) 都在直线 y = x 上',
               14.5, MUTED))

    # ---- 面板 A
    ga = Grid(112, 448, CELL)
    c.add(text(112, 152, '(A) 允许接触对角线  →  C_{n} 条', 17.5, BLUE, weight='700'))
    draw_grid(c, ga, 0, 4, 0, 4)
    diagonal(c, ga, 0, 4)
    pathA = [(0, 0), (1, 0), (1, 1), (2, 1), (3, 1), (3, 2), (3, 3), (4, 3), (4, 4)]
    c.add(poly(ga.pts(pathA), BLUE, 3.0))
    draw_ticks(c, ga, 0, 4, 0, 4)
    for q in [(1, 1), (3, 3)]:
        dot(c, ga, q[0], q[1], GREEN, 5.0)
    p = ga.p(4, 4)
    c.add(circle(p[0], p[1], 6.0, '#ffffff', INK, 2.2))
    c.add(text(112, 486, '路径可以经过 y = x 上的点（图中绿点），这就是 Dyck 路径',
               14.5, MUTED))

    # ---- 面板 B
    gb = Grid(552, 448, CELL)
    c.add(text(552, 152, '(B) 内部不接触对角线  →  2·C_{n-1} 条', 17.5, VIOLET, weight='700'))
    draw_grid(c, gb, 0, 4, 0, 4)
    diagonal(c, gb, 0, 4)
    pathB = [(0, 0), (1, 0), (2, 0), (2, 1), (3, 1), (4, 1), (4, 2), (4, 3), (4, 4)]
    c.add(poly(gb.pts(pathB), BLUE, 3.0))
    mirror = [(y, x) for (x, y) in pathB]
    c.add(poly(gb.pts(mirror), VIOLET, 3.0, dash='9 6'))
    draw_ticks(c, gb, 0, 4, 0, 4)
    p = gb.p(4, 4)
    c.add(circle(p[0], p[1], 6.0, '#ffffff', INK, 2.2))
    c.add(text(552, 486, '两条路径只在两端碰到对角线，且互为关于 y = x 的镜像',
               14.5, MUTED))
    c.add(text(552, 510, '→ 下方 C_{n-1} 条 + 上方 C_{n-1} 条 = 2·C_{n-1} 条', 14.5, VIOLET))

    # ---- 图例
    c.add(line(112, 540, 152, 540, BLUE, 3.0))
    c.add(text(160, 545, '不越过对角线（下方）', 14, INK))
    c.add(line(370, 540, 410, 540, VIOLET, 3.0, dash='9 6'))
    c.add(text(418, 545, '严格在上方（镜像，虚线）', 14, INK))
    c.add(line(690, 540, 730, 540, DIAG, 2.0, dash='7 6'))
    c.add(text(738, 545, '对角线 y = x', 14, INK))
    return c


# ---------------------------------------------------------------- 图 2
def fig2():
    W, H, CELL = 1010, 672, 76
    c = Canvas(W, H)
    c.add(text(28, 42, '反射原理：越界路径  ⟷  从 (−1, 1) 到 (n, n) 的路径（n = 4）',
               21, INK, weight='700'))
    c.add(text(28, 70, '把「起点到首次碰到 y = x + 1 的那一段」关于该直线作镜像，越界路径与'
                       '从 (−1,1) 出发的路径一一对应', 14.5, MUTED))
    c.add(text(28, 100, 'P：路径首次碰到直线 y = x + 1 的点（唯一）', 14, RED))

    g = Grid(216, 574, CELL)
    draw_grid(c, g, -1, 5, 0, 5)
    diagonal(c, g, 0, 5)
    # 约束线 y = x + 1
    q1, q2 = g.p(-1, 0), g.p(4, 5)
    c.add(line(q1[0], q1[1], q2[0], q2[1], CONST, 2.6, cap='round'))
    c.add(text(q2[0] - 6, q2[1] - 10, 'y = x + 1', 15, CONST, 'end', weight='700'))

    bad = [(0, 0), (1, 0), (1, 1), (1, 2), (2, 2), (3, 2), (3, 3), (4, 3), (4, 4)]
    refl = [(-1, 1), (-1, 2), (0, 2), (1, 2)]
    c.add(poly(g.pts(refl), GREEN, 3.0, dash='9 6'))
    c.add(poly(g.pts(bad), BLUE, 3.0))
    draw_ticks(c, g, -1, 5, 0, 5)

    # 关键点
    dot(c, g, 0, 0, INK, 5.6)
    dot(c, g, -1, 1, GREEN, 5.6)
    dot(c, g, 1, 2, RED, 6.4)
    pP = g.p(1, 2)
    c.add(text(pP[0] - 12, pP[1] - 12, 'P', 17, RED, 'end', weight='700'))

    c.add(text(g.p(0, 0)[0] + 8, g.p(0, 0)[1] + 44, '起点 (0, 0)', 14, INK))
    c.add(text(34, 470, '反射后的起点', 14, GREEN, 'start', weight='700'))
    c.add(text(34, 492, '(−1, 1)', 14, GREEN, 'start', weight='700'))

    # 注释：① 原始段、② 反射段
    c.add(text(344, 500, '① 起点 → P（此前未越界）', 14, BLUE))
    c.add(text(134, 380, '② 关于 y = x + 1 作镜像', 14, GREEN))
    c.add(arrow(170, 388, 170, 404, GREEN, 1.6, marker='arwG'))

    # 右侧说明卡
    x0, y0, x1, y1 = 620, 122, 984, 596
    c.add(rect(x0, y0, x1 - x0, y1 - y0, PANEL, '#cbd5e1', 1.4, rx=12))
    tx = x0 + 22
    c.add(text(tx, y0 + 40, '计数（n = 4）', 17, INK, weight='700'))
    rows = [
        ('全部路径', 'C(2n, n) = C(8, 4) = 70', INK),
        ('坏路径（越过对角线）', '= 从 (−1, 1) 到 (n, n) 的路径数', RED),
        ('　↳ 需要 n+1 个 E、n−1 个 N', 'C(2n, n−1) = C(8, 3) = 56', RED),
        ('合法的 Dyck 路径', 'C_{n} = 70 − 56 = 14  ✓', BLUE),
    ]
    yy = y0 + 74
    for lab, val, col in rows:
        c.add(circle(tx + 4, yy - 5, 3.4, col))
        c.add(text(tx + 16, yy, lab, 14.5, INK))
        c.add(text(tx + 16, yy + 21, val, 13.5, col))
        yy += 52

    c.add(line(x0 + 22, y0 + 300, x1 - 22, y0 + 300, '#cbd5e1', 1.2))
    c.add(text(x0 + 22, y0 + 330, '为什么是双射？', 15.5, INK, weight='700'))
    for i, s in enumerate([
            '· 反射段与 P 之后的段拼起来，步型不变，',
            '  仍然是一条格点路径；',
            '· 反过来，从 (−1,1) 到 (n,n) 的任意路径，',
            '  因 y − x − 1 由 +1 变到 −1，必经过',
            '  y = x + 1，取首次交点再反射即得原路径。']):
        c.add(text(x0 + 22, y0 + 356 + i * 22, s, 13.5, MUTED))
    return c


# ---------------------------------------------------------------- 图 3
def fig3():
    W, H, CELL = 900, 620, 86
    c = Canvas(W, H)
    c.add(text(28, 42, '按「首次返回对角线」分解：C_{n} = Σ_{k=1..n} C_{k-1} · C_{n-k}',
               21, INK, weight='700'))
    c.add(text(28, 70, '示例 n = 4、首次返回点 k = 2；把中间段平移后即为规模 k−1 的 Dyck 路径',
               14.5, MUTED))

    g = Grid(150, 508, CELL)
    draw_grid(c, g, 0, 4, 0, 4)
    diagonal(c, g, 0, 4)
    head = [(0, 0), (1, 0), (2, 0), (2, 1), (2, 2)]
    tail = [(2, 2), (3, 2), (4, 2), (4, 3), (4, 4)]
    c.add(poly(g.pts(head), RED, 3.2))
    c.add(poly(g.pts(tail), BLUE, 3.2))
    draw_ticks(c, g, 0, 4, 0, 4)

    # 高亮「去掉首尾 E、N 后的中间段」
    h1 = g.p(1, 0)
    h2 = g.p(2, 1)
    c.add(rect(h1[0] - 6, h2[1] - 6, (h2[0] - h1[0]) + 12, (h1[1] - h2[1]) + 12,
              'rgba(220,38,38,0.10)', RED, 1.4, dash='6 5', rx=8))

    dot(c, g, 0, 0, INK, 5.6)
    dot(c, g, 2, 2, RED, 6.4)
    pP = g.p(2, 2)
    bg_text(c, pP[0] - 14, pP[1] - 20, 130, 23, '首次返回 (k, k)', 14.5, RED, 'end', weight='700')
    c.add(arrow(pP[0] - 8, pP[1] - 24, pP[0] - 3, pP[1] - 9, RED, 1.5, marker='arwR'))
    c.add(text(252, 546, '①', 15, RED, weight='700'))
    c.add(text(g.p(4, 2)[0] + 12, g.p(4, 2)[1] + 6, '②', 15, BLUE, weight='700'))

    x0, y0, x1, y1 = 560, 118, 872, 430
    c.add(rect(x0, y0, x1 - x0, y1 - y0, PANEL, '#cbd5e1', 1.4, rx=12))
    tx = x0 + 22
    yy = y0 + 40
    c.add(text(tx, yy, '两段各自计数', 17, INK, weight='700'))
    yy += 34
    c.add(circle(tx + 4, yy - 5, 3.4, RED))
    c.add(text(tx + 16, yy, '① 首达段 = E + 下方路径 + N', 14.5, INK))
    c.add(text(tx + 16, yy + 21, '→ 有 C_{k-1} 种（k = 2 → C_{1} = 1）', 13.5, RED))
    yy += 62
    c.add(circle(tx + 4, yy - 5, 3.4, BLUE))
    c.add(text(tx + 16, yy, '② 尾段 = (k,k) 到 (n,n) 的 Dyck 路径', 14.5, INK))
    c.add(text(tx + 16, yy + 21, '→ 有 C_{n-k} 种（n−k = 2 → C_{2} = 2）', 13.5, BLUE))
    yy += 66
    c.add(line(x0 + 22, yy, x1 - 22, yy, '#cbd5e1', 1.2))
    c.add(text(tx, yy + 30, '本例：C_{1} × C_{2} = 1 × 2 = 2', 14.5, INK))
    c.add(text(tx, yy + 56, '对所有 k 求和即得', 14.5, INK))
    c.add(text(tx, yy + 80, 'C_{n} = Σ C_{k-1} C_{n-k}', 15.5, VIOLET, weight='700'))

    c.add(text(150, 570, '红框内为中间段：去掉首尾的 E、N 后平移 (−1, 0)，'
                         '即 (0,0) → (k−1, k−1) 的 Dyck 路径', 14, MUTED))
    return c


# ---------------------------------------------------------------- 图 4
def fig4():
    paths = [
        ['E', 'E', 'E', 'N', 'N', 'N'],
        ['E', 'E', 'N', 'E', 'N', 'N'],
        ['E', 'E', 'N', 'N', 'E', 'N'],
        ['E', 'N', 'E', 'E', 'N', 'N'],
        ['E', 'N', 'E', 'N', 'E', 'N'],
    ]
    CIRCLED = '①②③④⑤'
    W, H, CELL = 1140, 400, 42
    c = Canvas(W, H)
    c.add(text(28, 40, 'n = 3 的全部 5 条 Dyck 路径（不越过 y = x，允许落在 y = x 上）',
               21, INK, weight='700'))
    c.add(text(28, 68, '共 C_{3} = 5 条；绿点是路径经过对角线 y = x 的中间点', 14.5, MUTED))

    for i, steps in enumerate(paths):
        ox = 62 + i * 216
        g = Grid(ox, 300, CELL)
        c.add(text(ox, 132, '%s %s' % (CIRCLED[i], ' '.join(steps)), 15, INK, weight='700'))
        draw_grid(c, g, 0, 3, 0, 3)
        diagonal(c, g, 0, 3)
        pts = [(0, 0)]
        x = y = 0
        for s in steps:
            if s == 'E':
                x += 1
            else:
                y += 1
            pts.append((x, y))
        c.add(poly(g.pts(pts), BLUE, 2.8))
        for (px, py) in pts[1:-1]:
            if px == py:
                dot(c, g, px, py, GREEN, 3.8)
        p = g.p(3, 3)
        c.add(circle(p[0], p[1], 5.5, '#ffffff', INK, 2.0))
        c.add(text(ox + 3 * CELL / 2.0, 336, '#E ≥ #N 恒成立', 13, MUTED, 'middle'))

    c.add(line(62, 372, 96, 372, BLUE, 2.8))
    c.add(text(104, 377, 'Dyck 路径（E 右、N 上）', 14, INK))
    c.add(line(330, 372, 364, 372, DIAG, 2.0, dash='7 6'))
    c.add(text(372, 377, '对角线 y = x', 14, INK))
    c.add(circle(560, 372, 4.2, GREEN))
    c.add(text(572, 377, '允许接触对角线的点', 14, INK))
    return c


# ---------------------------------------------------------------- 输出
FIGS = [
    (fig1, 940, 566, 'fig1-两种读法'),
    (fig2, 1010, 672, 'fig2-反射原理'),
    (fig3, 900, 620, 'fig3-首次返回分解'),
    (fig4, 1140, 400, 'fig4-n3的五条路径'),
]


def main():
    for i, (fn, w, h, name) in enumerate(FIGS, start=1):
        svg_name = fn().save(name)
        html = ('<!doctype html><html><head><meta charset="utf-8">'
                '<style>html,body{margin:0;padding:0;background:#ffffff;overflow:hidden}'
                'img{display:block;width:%dpx;height:%dpx}</style></head>'
                '<body><img src="%s"></body></html>'
                % (w, h, urllib.parse.quote(svg_name)))
        hp = os.path.join(HERE, '_render_fig%d.html' % i)
        with open(hp, 'w', encoding='utf-8') as f:
            f.write(html)
        print('OK', svg_name, '%dx%d' % (w, h))
    print('---')
    for i, (fn, w, h, name) in enumerate(FIGS, start=1):
        url = 'file:///' + urllib.parse.quote(
            os.path.join(HERE, '_render_fig%d.html' % i).replace('\\', '/'))
        print('FIG%d|%s|%s|%d|%d' % (i, name, url, w, h))


if __name__ == '__main__':
    main()
