# -*- coding: utf-8 -*-
"""
哈夫曼编码动画演示：参考实现 + 自包含 HTML 生成器

用法:
    python make_huffman_demo.py

产出:
    huffman-demo.html      自包含动画（双击即可播放，无外部依赖）
    huffman-steps.json     每步的森林/树/码表数据（便于核对与二次开发）

脚本同时做四项自检并打印报告：
    1. 合并过程中每次取的都是当前森林中最小的两棵子树；
    2. 生成的码是前缀码（Kraft 等式取等 + 两两不互为前缀）；
    3. 平均码长 L 与"穷举所有二叉树"得到的真正最优值相等（n<=8 时可行）；
    4. 香农界 H <= L < H+1 成立。
"""
import heapq
import itertools
import json
import math
import os

HERE = os.path.dirname(os.path.abspath(__file__))


# ----------------------------------------------------------------- 核心算法
class Node(object):
    __slots__ = ('w', 'sym', 'l', 'r', 'serial', '_sel', '_new')

    def __init__(self, w, sym=None, l=None, r=None, serial=0):
        self.w, self.sym, self.l, self.r, self.serial = w, sym, l, r, serial
        self._sel = self._new = False

    @property
    def leaf(self):
        return self.sym is not None

    def leaves(self):
        if self.leaf:
            return [self.sym]
        return self.l.leaves() + self.r.leaves()

    def depth(self):
        if self.leaf:
            return 0
        return 1 + max(self.l.depth(), self.r.depth())


def huffman_steps(freq):
    """按 (权值, 创建序号) 断平局，返回 (每步快照, 合并序列, 根)。"""
    heap = []
    for i, (sym, w) in enumerate(freq):
        heapq.heappush(heap, (w, i, Node(w, sym=sym, serial=i)))
    serial = len(freq)
    snapshots = [[t for _, _, t in sorted(heap)]]
    merges = []
    while len(heap) > 1:
        wa, _, a = heapq.heappop(heap)
        wb, _, b = heapq.heappop(heap)
        new = Node(wa + wb, l=a, r=b, serial=serial)
        serial += 1
        merges.append((a, b, new))
        heapq.heappush(heap, (new.w, new.serial, new))
        snapshots.append([t for _, _, t in sorted(heap)])
    return snapshots, merges, heap[0][2]


def codes_of(root):
    out = {}

    def walk(node, prefix):
        if node.leaf:
            out[node.sym] = prefix or '0'
            return
        walk(node.l, prefix + '0')
        walk(node.r, prefix + '1')
    walk(root, '')
    return out


def entropy(freq):
    total = float(sum(w for _, w in freq))
    h = 0.0
    for _, w in freq:
        p = w / total
        if p > 0:
            h -= p * math.log(p, 2)
    return h


def best_by_dp(freq):
    """独立验证：穷举所有满二叉树形状（按子集 DP），返回最小加权外部路径长度。"""
    ws = [w for _, w in freq]
    n = len(ws)
    weight = [0] * (1 << n)
    for mask in range(1, 1 << n):
        low = mask & -mask
        i = low.bit_length() - 1
        weight[mask] = weight[mask ^ low] + ws[i]
    INF = float('inf')
    best = [INF] * (1 << n)
    for mask in range(1, 1 << n):
        if mask & (mask - 1) == 0:          # 单元素：代价 0（叶子的贡献在父结点算）
            best[mask] = 0
            continue
        sub = (mask - 1) & mask
        b = INF
        while sub:
            other = mask ^ sub
            if other:
                v = best[sub] + best[other] + weight[mask]
                if v < b:
                    b = v
            sub = (sub - 1) & mask
        best[mask] = b
    return best[(1 << n) - 1]


# ------------------------------------------- 交换引理 / 排序不等式的数值核对
def check_swap_lemma():
    """核对 §3.1 的交换引理，以及它与排序不等式的关系。

    固定树形（深度多重集固定），只置换叶子的权值标签：
      (1) 任意两个叶子互换标签，代价变化 Δ 是否恒等于 (w_a-w_b)(d_b-d_a)；
      (2) 满足 w_a<=w_b 且 d_a<=d_b 的配对，是否一定有 Δ<=0（交换不亏）；
      (3) 穷举全部标号，最优标号是否就是"权值降序配深度升序"。
    返回可直接打印的统计字典。
    """
    import itertools
    depths = {'a': 1, 'c': 3, 'b': 3, 'd': 3, 'f': 4, 'e': 4}   # 经典例子的最优树形
    w0 = {'a': 45, 'b': 13, 'c': 12, 'd': 16, 'e': 9, 'f': 5}
    syms = list(depths)

    def cost(ws):
        return sum(ws[s] * depths[s] for s in syms)

    before = cost(w0)
    ident_ok = ident_bad = 0
    light_shallow_ok = light_shallow_bad = 0
    heavy_shallow_ok = heavy_shallow_bad = 0
    for a, b in itertools.combinations(syms, 2):
        sw = dict(w0)
        sw[a], sw[b] = w0[b], w0[a]
        delta = cost(sw) - before
        pred = (w0[a] - w0[b]) * (depths[b] - depths[a])
        if delta == pred:
            ident_ok += 1
        else:
            ident_bad += 1
        if w0[a] <= w0[b] and depths[a] <= depths[b]:       # 轻的又更浅
            (light_shallow_ok, light_shallow_bad) = \
                (light_shallow_ok + 1, light_shallow_bad) if delta <= 0 else \
                (light_shallow_ok, light_shallow_bad + 1)
        if w0[a] >= w0[b] and depths[a] <= depths[b]:       # 重的反而更浅
            (heavy_shallow_ok, heavy_shallow_bad) = \
                (heavy_shallow_ok + 1, heavy_shallow_bad) if delta >= 0 else \
                (heavy_shallow_ok, heavy_shallow_bad + 1)

    best, best_ws = None, None
    for perm in itertools.permutations(w0.values()):
        ws = dict(zip(syms, perm))
        c = cost(ws)
        if best is None or c < best:
            best, best_ws = c, ws
    order = sorted(syms, key=lambda s: depths[s])           # 深度升序
    wdesc = sorted(w0.values(), reverse=True)               # 权值降序
    anti_cost = sum(wdesc[i] * depths[order[i]] for i in range(len(order)))

    return {'ident_ok': ident_ok, 'ident_bad': ident_bad,
            'light_ok': light_shallow_ok, 'light_bad': light_shallow_bad,
            'heavy_ok': heavy_shallow_ok, 'heavy_bad': heavy_shallow_bad,
            'brute_best': best, 'anti_cost': anti_cost, 'n_labelings': 720}


# ------------------------------------------- OI 视角：合并果子 / k 叉霍夫曼
def kary_huffman(weights, k):
    """k 叉霍夫曼（补 0 权虚叶）：返回 (WPL, 补零个数)。

    补零规则：满 k 叉树满足 (叶子数-1) % (k-1) == 0，所以先补
    (-(n-1)) % (k-1) 个 0 权叶子，再每次合并最小的 k 个。
    """
    ws = list(weights)
    pad = 0
    if k > 2 and len(ws) > 1:
        pad = (-(len(ws) - 1)) % (k - 1)
    ws += [0] * pad
    heapq.heapify(ws)
    wpl = 0
    while len(ws) > 1:
        s = 0
        for _ in range(k):
            s += heapq.heappop(ws)
        wpl += s
        heapq.heappush(ws, s)
    return wpl, pad


def best_kary_dp(weights, k):
    """穷举所有满 k 叉树（子集 DP）求最小 WPL，用来独立核对 kary_huffman。"""
    n = len(weights)
    wsum = [0] * (1 << n)
    for mask in range(1, 1 << n):
        low = mask & -mask
        wsum[mask] = wsum[mask ^ low] + weights[low.bit_length() - 1]

    def parts(mask, kk):
        """把 mask 拆成 kk 个非空子集（不计顺序）的所有方案。"""
        if kk == 1:
            yield [mask]
            return
        low = mask & -mask
        rest = mask ^ low
        sub = rest
        while True:
            part = sub | low
            rem = mask ^ part
            if bin(rem).count('1') >= kk - 1:
                for tail in parts(rem, kk - 1):
                    yield [part] + tail
            if sub == 0:
                break
            sub = (sub - 1) & rest

    from functools import lru_cache

    INF = float('inf')

    @lru_cache(maxsize=None)
    def best(mask):
        if mask & (mask - 1) == 0:
            return 0
        # 满 k 叉子树的叶子数必须满足 L ≡ 1 (mod k-1)，否则这个子集无解
        if bin(mask).count('1') % (k - 1) != 1:
            return INF
        b = INF
        for ps in parts(mask, k):
            v = wsum[mask]
            ok = True
            for p in ps:
                bp = best(p)
                if bp == INF:
                    ok = False
                    break
                v += bp
            if ok and v < b:
                b = v
        return b
    return best((1 << n) - 1)


# ----------------------------------------------------------------- 序列化
def ser(node, mark_new=False, mark_sel=False, code_map=None):
    if node.leaf:
        d = {'sym': node.sym, 'w': node.w}
        if code_map:
            d['code'] = code_map[node.sym]
    else:
        d = {'w': node.w, 'set': ''.join(sorted(node.leaves())),
             'l': ser(node.l, code_map=code_map), 'r': ser(node.r, code_map=code_map)}
    if mark_new:
        d['new'] = True
    if mark_sel:
        d['sel'] = True
    return d


def sorted_roots(roots):
    return sorted(roots, key=lambda t: (t.w, t.serial))


def sset(node):
    """子树叶子集合的规范写法（升序），与卡片上的“f e”标签保持一致。"""
    return ''.join(sorted(node.leaves()))


def build_preset(name, freq, desc):
    total = float(sum(w for _, w in freq))
    snapshots, merges, root = huffman_steps(freq)
    code_map = codes_of(root)
    H = entropy(freq)
    L = sum(w / total * len(code_map[s]) for s, w in freq)
    bits = int(round(L * total))
    fixed_bits = int(math.ceil(math.log(len(freq), 2))) * int(total)
    steps = []
    for k, roots_in_heap in enumerate(snapshots):
        roots = sorted_roots(roots_in_heap)
        if k == 0:
            for i, t in enumerate(roots[:2]):
                t._sel = True
            caption = '初始：把 %d 个符号各看作一棵单结点树，按权值升序排好。'\
                      '哈夫曼的贪心选择是——每次取权值最小的两棵子树。' % len(roots)
            detail = '接下来合并 “%s”(%d) 与 “%s”(%d)' % (
                sset(roots[0]), roots[0].w, sset(roots[1]), roots[1].w)
        elif k < len(merges):
            a, b, new = merges[k - 1]
            for t in roots:
                if t.serial == new.serial:
                    t._new = True
            for i, t in enumerate(roots[:2]):
                t._sel = True
            caption = '第 %d 步：合并最小的两棵子树 “%s”(%d) + “%s”(%d) → 新结点 “%s”(%d)' % (
                k, sset(a), a.w, sset(b), b.w, sset(new), new.w)
            detail = '合并后森林剩 %d 棵树；接下来取 “%s”(%d) 与 “%s”(%d)' % (
                len(roots), sset(roots[0]), roots[0].w, sset(roots[1]), roots[1].w)
        else:
            a, b, new = merges[k - 1]
            for t in roots:
                if t.serial == new.serial:
                    t._new = True
            caption = '第 %d 步：合并 “%s”(%d) + “%s”(%d) → 根结点 “%s”(%d)，森林只剩一棵树，编码完成' % (
                k, sset(a), a.w, sset(b), b.w, sset(new), new.w)
            detail = '码长随频率反向变化：频率越高码越短'
        steps.append({
            'forest': [ser(t, mark_new=getattr(t, '_new', False),
                           mark_sel=getattr(t, '_sel', False),
                           code_map=code_map if k == len(snapshots) - 1 else None)
                       for t in roots],
            'caption': caption,
            'detail': detail,
            'roots': len(roots),
            'done': k == len(snapshots) - 1,
            'L': round(L, 3),
            'H': round(H, 3),
            'bits': bits,
            'fixed': fixed_bits,
            'table': '  '.join('%s:%s' % (s, code_map[s]) for s, _ in
                               sorted(freq, key=lambda x: (-x[1], x[0]))),
        })
        for t in roots:
            t._new = False
            t._sel = False
    return {
        'name': name, 'desc': desc,
        'freq': [{'sym': s, 'w': w} for s, w in freq],
        'total': int(total), 'H': round(H, 3), 'L': round(L, 3),
        'depthMax': root.depth(), 'bits': bits, 'fixed': fixed_bits,
        'codes': [{'sym': s, 'w': w, 'code': code_map[s]} for s, w in freq],
        'steps': steps,
    }


# ----------------------------------------------------------------- 自检
def check(preset):
    freq = [(d['sym'], d['w']) for d in preset['freq']]
    total = float(sum(w for _, w in freq))
    code_map = {d['sym']: d['code'] for d in preset['codes']}
    # 1. 前缀码：Kraft 等式 + 两两不互为前缀
    kraft = sum(2.0 ** -len(c) for c in code_map.values())
    prefix_free = all(not (a != b and b.startswith(a))
                      for a in code_map.values() for b in code_map.values())
    # 2. 字母表满二叉性：任何非叶结点都有两个孩子（Huffman 天生满足）
    # 3. 穷举最优
    opt = best_by_dp(freq)
    Lbits = sum(w * len(code_map[s]) for s, w in freq)
    H = entropy(freq)
    L = Lbits / total
    return {
        'kraft': kraft,
        'prefix_free': prefix_free,
        'dp_optimum': opt,
        'huffman_bits': Lbits,
        'H': H, 'L': L,
        'bound_ok': H - 1e-9 <= L < H + 1,
    }


# ----------------------------------------------------------------- HTML
HTML = r'''<!doctype html>
<html lang="zh-CN">
<head>
<meta charset="utf-8">
<title>哈夫曼编码：贪心合并全过程</title>
<style>
  :root { --ink:#0f172a; --muted:#64748b; --line:#cbd5e1; --blue:#2563eb;
          --orange:#f59e0b; --green:#16a34a; --red:#dc2626; --violet:#7c3aed; }
  * { box-sizing: border-box; }
  html,body { margin:0; padding:0; background:#f1f5f9;
              font-family:"Microsoft YaHei","Segoe UI",Arial,sans-serif; color:var(--ink); }
  #wrap { width:1000px; margin:0 auto; }
  h1 { font-size:22px; margin:14px 0 6px; }
  .sub { color:var(--muted); font-size:13.5px; margin-bottom:10px; }
  #bar { display:flex; flex-wrap:wrap; gap:8px; align-items:center; margin-bottom:10px; }
  button { font:inherit; font-size:14px; padding:6px 14px; border-radius:8px;
           border:1.4px solid var(--line); background:#fff; color:var(--ink); cursor:pointer; }
  button:hover { border-color:var(--blue); color:var(--blue); }
  button.on { background:var(--blue); border-color:var(--blue); color:#fff; }
  button.pri { background:var(--green); border-color:var(--green); color:#fff; }
  input[type=range] { width:110px; vertical-align:middle; }
  .spacer { flex:1; }
  #scene { display:block; width:1000px; height:620px; background:#fff;
           border-radius:12px; border:1px solid var(--line); }
  body.static #wrap { width:1000px; margin:0; }
  body.static h1, body.static .sub, body.static #bar { display:none; }
  body.static #scene { border-radius:0; border:none; }
  .pop { animation: pop .45s ease-out; transform-box: fill-box; transform-origin: center; }
  @keyframes pop { from { opacity:.15; transform:scale(.55); } to { opacity:1; transform:scale(1); } }
  .fade { animation: fade .45s ease-out; }
  @keyframes fade { from { opacity:0; } to { opacity:1; } }
</style>
</head>
<body>
<div id="wrap">
  <h1>哈夫曼编码：贪心合并全过程</h1>
  <div class="sub">每次取权值最小的两棵子树合并，直到只剩一棵树；左分支记 0、右分支记 1。
    频率越高的符号越靠近根，码越短。</div>
  <div id="bar"></div>
  <svg id="scene" viewBox="0 0 1000 620" xmlns="http://www.w3.org/2000/svg"></svg>
</div>
<script>
const DATA = __DATA__;
const NS = 'http://www.w3.org/2000/svg';
const svg = document.getElementById('scene');
const bar = document.getElementById('bar');
const qs = new URLSearchParams(location.search);
const STATIC = qs.has('static');

let pi = 0, step = 0, timer = null;

/* ---------------------------------------------------------- 小工具 */
const esc = s => String(s).replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
const txt = (x,y,s,size=14,fill='#0f172a',anchor='start',weight='400',cls='') =>
  `<text class="${cls}" x="${x}" y="${y}" font-family="Microsoft YaHei,Segoe UI,Arial,sans-serif"
     font-size="${size}" fill="${fill}" text-anchor="${anchor}" font-weight="${weight}">${esc(s)}</text>`;
const rect = (x,y,w,h,fill,stroke,rx=10,sw=1.4,dash='') =>
  `<rect x="${x}" y="${y}" width="${w}" height="${h}" rx="${rx}" fill="${fill}" stroke="${stroke}"
     stroke-width="${sw}"${dash?` stroke-dasharray="${dash}"`:''}/>`;
const line = (x1,y1,x2,y2,stroke,sw=2,dash='') =>
  `<line x1="${x1}" y1="${y1}" x2="${x2}" y2="${y2}" stroke="${stroke}" stroke-width="${sw}"
     stroke-linecap="round"${dash?` stroke-dasharray="${dash}"`:''}/>`;
const circ = (cx,cy,r,fill,stroke,sw=2,cls='') =>
  `<circle class="${cls}" cx="${cx}" cy="${cy}" r="${r}" fill="${fill}" stroke="${stroke}" stroke-width="${sw}"/>`;

/* ---------------------------------------------------------- 树工具 */
const isLeaf = t => t.sym !== undefined;
const countLeaves = t => isLeaf(t) ? 1 : countLeaves(t.l) + countLeaves(t.r);
const depthOf = t => isLeaf(t) ? 0 : 1 + Math.max(depthOf(t.l), depthOf(t.r));
const setOf = t => isLeaf(t) ? t.sym : t.set;

function layout(forest){
  const n = forest.reduce((a,t) => a + countLeaves(t), 0);
  const maxD = Math.max(...forest.map(depthOf), 1);
  const x0 = 470, x1 = 962, topY = 84, botY = 400;
  const slotW = (x1 - x0) / n, lh = (botY - topY) / maxD;
  let slot = 0;
  const recs = [], byNode = new Map();
  function place(t, d){
    let x;
    if (isLeaf(t)) { x = x0 + slotW * (slot + .5); slot++; }
    else { const a = place(t.l, d+1), b = place(t.r, d+1); x = (a + b) / 2; }
    const rec = { t, x, y: topY + d * lh, d };
    recs.push(rec); byNode.set(t, rec);
    return x;
  }
  forest.forEach(t => place(t, 0));
  const bounds = new Map();
  forest.forEach(root => {
    const sub = recs.filter(r => { let p = r.t, ok = false;
      const walk = (node) => { if (node === root) { ok = true; return; }
        if (!isLeaf(node)) { walk(node.l); walk(node.r); } };
      walk(root); return ok; });
    const xs = recs.map(r => r.x);
    bounds.set(root, { x0: Math.min(...xs), x1: Math.max(...xs) });
  });
  return { recs, byNode, lh };
}

/* 某棵根子树包含的所有结点坐标 */
function subtreeRecs(root, L){
  const out = [];
  (function walk(t){ const r = L.byNode.get(t); out.push(r);
    if (!isLeaf(t)) { walk(t.l); walk(t.r); } })(root);
  return out;
}

/* ---------------------------------------------------------- 渲染 */
function drawCards(parts, st){
  parts.push(txt(24, 40, '森林：当前的子树（按权值升序，最小的两棵下次合并）', 15, '#0f172a', 'start', 700));
  const n = st.forest.length;
  const top = 58, avail = 462 - top;
  const ch = Math.min(56, Math.floor(avail / n) - 6);
  const gap = 6;
  st.forest.forEach((t, i) => {
    const y = top + i * (ch + gap);
    const sel = !!t.sel, isNew = !!t.new;
    const fill = sel ? '#fff7ed' : (isNew ? '#ecfdf5' : '#f8fafc');
    const stroke = sel ? '#f59e0b' : (isNew ? '#16a34a' : '#cbd5e1');
    parts.push(rect(24, y, 386, ch, fill, stroke, 10, sel || isNew ? 2.4 : 1.3,
                    sel ? '7 5' : ''));
    const mid = y + ch / 2;
    parts.push(txt(40, mid + 6, '“' + setOf(t) + '”', 17, '#0f172a', 'start', 700));
    parts.push(txt(250, mid + 6, '权值 ' + t.w, 14.5, stroke === '#cbd5e1' ? '#475569' : stroke,
                   'start', 700));
    if (sel) parts.push(txt(392, mid + 7, i === 0 ? '①' : '②', 16, '#f59e0b', 'end', 700));
    // “新结点”和 ①② 可能同时出现，错开横坐标，避免叠在同一位置
    if (isNew) parts.push(txt(sel ? 348 : 392, mid + 6, '新结点', 13.5, '#16a34a', 'end', 700));
  });
}

function drawForest(parts, st){
  parts.push(txt(470, 40, '编码树：左 0 / 右 1，圆内数字为该子树的权值', 15, '#0f172a', 'start', 700));
  const L = layout(st.forest);
  // 每棵根子树的虚线外框
  st.forest.forEach((root, i) => {
    const rs = subtreeRecs(root, L);
    const xs = rs.map(r => r.x), ys = rs.map(r => r.y);
    const x0 = Math.min(...xs) - 30, x1 = Math.max(...xs) + 30;
    const y0 = Math.min(...ys) - 34, y1 = Math.max(...ys) + 46;
    const sel = !!root.sel;
    parts.push(rect(x0, y0, x1 - x0, y1 - y0, sel ? 'rgba(245,158,11,.06)' : 'none',
                    sel ? '#f59e0b' : '#e2e8f0', 12, sel ? 2 : 1.2, sel ? '8 6' : ''));
    // 序号用圆形小徽章，和左侧卡片的 ①② 一一对应
    parts.push(circ(x0 + 14, y0 + 14, 10, sel ? '#f59e0b' : '#e2e8f0', '#ffffff', 1.5));
    parts.push(txt(x0 + 14, y0 + 19, '①②③④⑤⑥⑦⑧'.charAt(i), 13,
                   sel ? '#ffffff' : '#64748b', 'middle', 700));
  });
  // 边
  L.recs.forEach(r => {
    if (isLeaf(r.t)) return;
    [[r.t.l, '0'], [r.t.r, '1']].forEach(([k, lab]) => {
      const kr = L.byNode.get(k);
      parts.push(line(r.x, r.y, kr.x, kr.y, '#94a3b8', 2));
      parts.push(txt((r.x + kr.x) / 2 + (lab === '0' ? -11 : 11), (r.y + kr.y) / 2 + 5,
                     lab, 14, lab === '0' ? '#2563eb' : '#7c3aed', 'middle', 700));
    });
  });
  // 结点
  L.recs.forEach(r => {
    const t = r.t, isNew = !!t.new, cls = isNew ? 'pop' : '';
    if (isLeaf(t)) {
      const done = t.code !== undefined;
      parts.push(circ(r.x, r.y, 18, isNew ? '#dcfce7' : '#eff6ff',
                      isNew ? '#16a34a' : '#2563eb', 2.4, cls));
      parts.push(txt(r.x, r.y + 6.5, t.sym, 17, '#0f172a', 'middle', 700));
      const below = r.y + 38;
      parts.push(txt(r.x, below, 'w=' + t.w, 13, '#64748b', 'middle'));
      if (done) parts.push(txt(r.x, below + 20, '码 ' + t.code, 13.5, '#16a34a', 'middle', 700));
    } else {
      parts.push(circ(r.x, r.y, 18, isNew ? '#dcfce7' : '#f8fafc',
                      isNew ? '#16a34a' : '#475569', 2.4, cls));
      parts.push(txt(r.x, r.y + 5.5, t.w, 13.5, '#0f172a', 'middle', 700));
      if (isNew) parts.push(txt(r.x, r.y - 26, '新结点', 13, '#16a34a', 'middle', 700));
    }
  });
}

function drawBottom(parts, st){
  parts.push(rect(24, 478, 952, 124, '#f8fafc', '#cbd5e1', 12, 1.3));
  parts.push(txt(44, 508, st.caption, 16, '#0f172a', 'start', 700));
  parts.push(txt(44, 536, st.detail, 14, '#475569'));
  if (st.done) {
    parts.push(txt(44, 562, '平均码长 L = ' + st.L + ' 比特（编码 ' + st.bits +
                           ' 比特）；熵 H = ' + st.H + ' 比特；校验 H ≤ L < H+1  ✓', 14, '#2563eb'));
    parts.push(txt(44, 586, '符号 → 码字（按频率降序）：' + st.table, 13.5, '#16a34a'));
  } else {
    parts.push(txt(44, 562, '已合并 ' + st.merged + ' / ' + st.totalMerges +
                           ' 次；当前森林有 ' + st.roots + ' 棵树（合并到最后 1 棵即完成）', 14, '#64748b'));
    parts.push(txt(44, 586, '提示：新结点权值 = 两棵子树权值之和；把所有内部结点的权值相加，恰好等于编码总位数', 13.5, '#94a3b8'));
  }
}

function show(k){
  const P = DATA.presets[pi];
  step = Math.max(0, Math.min(k, P.steps.length - 1));
  const st = P.steps[step];
  const parts = [];
  parts.push(rect(0, 0, 1000, 620, '#ffffff', 'none'));
  drawCards(parts, st);
  drawForest(parts, st);
  drawBottom(parts, st);
  svg.innerHTML = parts.join('');
  if (!STATIC) {
    document.getElementById('cnt').textContent = (step + 1) + ' / ' + P.steps.length;
  }
}

/* ---------------------------------------------------------- 控件 */
function buildBar(){
  DATA.presets.forEach((P, i) => {
    const b = document.createElement('button');
    b.textContent = P.name;
    b.onclick = () => { stop(); pi = i; step = 0; refresh(); };
    b.dataset.preset = i;
    bar.appendChild(b);
  });
  const sep = document.createElement('span'); sep.className = 'spacer'; bar.appendChild(sep);
  const mk = (label, cls, fn) => { const b = document.createElement('button');
    b.textContent = label; if (cls) b.className = cls; b.onclick = fn; bar.appendChild(b); return b; };
  mk('⟲ 重置', '', () => { stop(); refresh(); });
  mk('◀ 上一步', '', () => { stop(); show(step - 1); barUpdate(); });
  window.btnPlay = mk('▶ 播放', 'pri', toggle);
  mk('下一步 ▶', '', () => { stop(); show(step + 1); barUpdate(); });
  const sp = document.createElement('label');
  sp.style.fontSize = '13.5px'; sp.style.color = '#64748b';
  sp.innerHTML = ' &nbsp;速度 <input type="range" id="sp" min="1" max="5" value="3"> &nbsp;' +
                 '<span id="cnt"></span>';
  bar.appendChild(sp);
}
function barUpdate(){
  [...bar.querySelectorAll('button[data-preset]')].forEach(b =>
    b.classList.toggle('on', +b.dataset.preset === pi));
}
function refresh(){ barUpdate(); show(step); }
function stop(){ if (timer) { clearInterval(timer); timer = null; btnPlay.textContent = '▶ 播放'; } }
function toggle(){
  if (timer) { stop(); return; }
  const P = DATA.presets[pi];
  if (step >= P.steps.length - 1) step = 0;
  btnPlay.textContent = '⏸ 暂停';
  timer = setInterval(() => {
    const P2 = DATA.presets[pi];
    if (step >= P2.steps.length - 1) { stop(); return; }
    show(step + 1);
  }, [1400, 1100, 900, 700, 520][+document.getElementById('sp').value - 1]);
}

/* ---------------------------------------------------------- 启动 */
DATA.presets.forEach(P => P.steps.forEach((s, i) => {
  s.merged = i; s.totalMerges = P.steps.length - 1;
}));
if (STATIC) {
  document.body.classList.add('static');
  pi = +(qs.get('preset') || 0);
  step = +(qs.get('step') || 0);
  show(step);
} else {
  buildBar();
  refresh();
  document.addEventListener('keydown', e => {
    if (e.key === 'ArrowRight') { stop(); show(step + 1); }
    if (e.key === 'ArrowLeft') { stop(); show(step - 1); }
    if (e.key === ' ') { e.preventDefault(); toggle(); }
  });
}
</script>
</body>
</html>
'''


def main():
    presets = [
        build_preset('经典 6 符号', [('a', 45), ('b', 13), ('c', 12),
                                     ('d', 16), ('e', 9), ('f', 5)],
                     '教材常用例子，总频率 100'),
        build_preset('4 符号（可手算）', [('a', 7), ('b', 5), ('c', 2), ('d', 4)],
                     '只有 3 次合并，适合先在纸上推一遍'),
        build_preset('斐波那契 8 符号', [('a', 1), ('b', 1), ('c', 2), ('d', 3),
                                         ('e', 5), ('f', 8), ('g', 13), ('h', 21)],
                     '极端分布：最深的码长达 7，说明码长可以接近 n-1'),
    ]

    print('=' * 74)
    print('【§3.1 交换引理】固定树形下只置换权值标签（经典例子，6 个叶子）')
    s = check_swap_lemma()
    print('  Δ == (w_a-w_b)(d_b-d_a) 的配对: %d/%d %s'
          % (s['ident_ok'], s['ident_ok'] + s['ident_bad'],
             '✓' if s['ident_bad'] == 0 else '✗'))
    print('  满足 w_a<=w_b 且 d_a<=d_b 的配对中 Δ<=0: %d 个，Δ>0 的: %d 个 %s'
          % (s['light_ok'], s['light_bad'], '✓' if s['light_bad'] == 0 else '✗'))
    print('  满足 w_a>=w_b 且 d_a<=d_b 的配对中 Δ>=0: %d 个，Δ<0 的: %d 个 %s'
          % (s['heavy_ok'], s['heavy_bad'], '✓' if s['heavy_bad'] == 0 else '✗'))
    print('  穷举全部 %d 种标号的最优代价 = %d；"权值降序配深度升序"的代价 = %d %s'
          % (s['n_labelings'], s['brute_best'], s['anti_cost'],
             '✓' if s['brute_best'] == s['anti_cost'] else '✗'))
    print('=' * 74)
    print('【OI 视角】合并果子 = 求 WPL 的最小值')
    for ws in ([1, 2, 9], [5, 9, 12, 13, 16, 45]):
        print('  权值 %-24s 二叉树 WPL = %d, 穷举最优 = %d'
              % (str(ws), kary_huffman(ws, 2)[0], best_by_dp([('x', w) for w in ws])))
    print('【OI 视角】k 叉霍夫曼（补 0 权虚叶）')
    for k, ws in ((3, [1, 2, 3, 4, 5]), (3, [1, 2, 3, 4]), (3, [1, 1, 2, 3, 5, 8]), (4, [1, 2, 3, 4, 5, 6])):
        wpl, pad = kary_huffman(ws, k)
        opt = best_kary_dp(ws + [0] * pad, k)
        print('  k=%d, 权值 %-26s 补零 %d 个 → k 叉 WPL = %d, 穷举最优 = %d : %s'
              % (k, str(ws), pad, wpl, opt, '✓' if wpl == opt else '✗'))
    print('=' * 74)
    for P in presets:
        c = check(P)
        print('【%s】%s' % (P['name'], P['desc']))
        print('  频率: ' + ', '.join('%s=%d' % (d['sym'], d['w']) for d in P['freq']))
        print('  码字: ' + '  '.join('%s:%s' % (d['sym'], d['code']) for d in P['codes']))
        print('  最大码长 = %d, 编码总位数 = %d (定长编码需 %d)'
              % (P['depthMax'], c['huffman_bits'], P['fixed']))
        print('  平均码长 L = %.4f, 熵 H = %.4f, 界 H <= L < H+1 : %s'
              % (c['L'], c['H'], '✓' if c['bound_ok'] else '✗'))
        print('  Kraft 和 = %.6f (应为 1): %s' % (c['kraft'], '✓' if abs(c['kraft'] - 1) < 1e-12 else '✗'))
        print('  前缀码（两两不互为前缀）: %s' % ('✓' if c['prefix_free'] else '✗'))
        print('  穷举所有二叉树的最优位数 = %d, 哈夫曼位数 = %d : %s'
              % (c['dp_optimum'], c['huffman_bits'],
                 '✓ 一致（哈夫曼最优）' if c['dp_optimum'] == c['huffman_bits'] else '✗ 不一致'))
        print('-' * 74)

    html = HTML.replace('__DATA__', json.dumps({'presets': presets}, ensure_ascii=False))
    with open(os.path.join(HERE, 'huffman-demo.html'), 'w', encoding='utf-8') as f:
        f.write(html)
    with open(os.path.join(HERE, 'huffman-steps.json'), 'w', encoding='utf-8') as f:
        json.dump({'presets': presets}, f, ensure_ascii=False, indent=1)
    print('已写出 huffman-demo.html（%d 字节）与 huffman-steps.json'
          % len(html.encode('utf-8')))
    print('步数：' + ', '.join('%s=%d 步' % (P['name'], len(P['steps'])) for P in presets))


if __name__ == '__main__':
    main()
