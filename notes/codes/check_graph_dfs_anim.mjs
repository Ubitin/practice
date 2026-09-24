// 校验「图DFS深度优先搜索动画.html」内嵌的 SIM 逻辑
// 抽 <script> → new Function(...,'document',...) 在 node 里跑（document 传 undefined，界面代码进不去）
// 再用三把独立尺子交叉验证：① 递归 DFS ② 模拟递归的显式栈 DFS ③ 朴素迭代 DFS（会不同，单独报告）
import fs from 'fs';

const path = process.argv[2] || '讲义/figures/图DFS深度优先搜索动画.html';
const html = fs.readFileSync(path, 'utf8');
const m = html.match(/<script>([\s\S]*?)<\/script>/);
if (!m) { console.log('FAIL: no <script>'); process.exit(1); }
const module = { exports: {} };
new Function('module', 'exports', 'document', m[1])(module, module.exports, undefined);
const SIM = module.exports;
if (!SIM || !SIM.run || !SIM.GRAPHS) { console.log('FAIL: SIM export'); process.exit(1); }

let fail = 0;
const bad = (s) => { console.log('  FAIL ' + s); fail++; };
const eq = (a, b) => JSON.stringify(a) === JSON.stringify(b);

// ---------- 尺子①：递归 DFS（按"先 start，再 0..n-1 递增补连通块"的顺序） ----------
function recDFS(adj, n, first) {
  const seen = new Array(n).fill(false), order = [];
  const go = (u) => { seen[u] = true; order.push(u); for (const v of adj[u]) if (!seen[v]) go(v); };
  if (first >= 0) go(first);
  for (let v = 0; v < n; v++) if (!seen[v]) go(v);
  return order;
}
// ---------- 尺子②：模拟递归的显式栈 DFS（不用递归，语义与递归完全一致） ----------
function iterDFS(adj, n, first) {
  const seen = new Array(n).fill(false), order = [];
  const run = (s) => {
    seen[s] = true; order.push(s);
    const st = [[s, 0]];
    while (st.length) {
      const fr = st[st.length - 1];
      if (fr[1] < adj[fr[0]].length) {
        const v = adj[fr[0]][fr[1]++];
        if (!seen[v]) { seen[v] = true; order.push(v); st.push([v, 0]); }
      } else st.pop();
    }
  };
  if (first >= 0) run(first);
  for (let v = 0; v < n; v++) if (!seen[v]) run(v);
  return order;
}
// ---------- 尺子③：朴素迭代 DFS（入栈即标记 + 一次全压邻居）—— 与递归语义【不同】，用于展示区别 ----------
function naiveDFS(adj, n, first) {
  const seen = new Array(n).fill(false), order = [];
  const run = (s) => {
    const st = [s]; seen[s] = true;
    while (st.length) {
      const u = st.pop(); order.push(u);
      for (let i = adj[u].length - 1; i >= 0; i--) {
        const v = adj[u][i];
        if (!seen[v]) { seen[v] = true; st.push(v); }
      }
    }
  };
  if (first >= 0) run(first);
  for (let v = 0; v < n; v++) if (!seen[v]) run(v);
  return order;
}

let totalFrames = 0, groups = 0, naiveDiff = 0;
for (const mode of Object.keys(SIM.GRAPHS)) {
  const g = SIM.GRAPHS[mode], n = g.n;
  for (let s = 0; s < n; s++) {
    const { frames } = SIM.run(mode, s);
    totalFrames += frames.length; groups++;
    const tag = '[' + mode + ' start=' + s + ']';
    const F = frames[frames.length - 1];

    // ① 访问顺序（含连通块）＝ 两把独立尺子
    const exp = recDFS(g.adj, n, s), exp2 = iterDFS(g.adj, n, s);
    if (!eq(F.order, exp)) bad(tag + ' 访问顺序与递归实现不符: ' + F.order + ' vs ' + exp);
    if (!eq(exp, exp2)) bad(tag + ' 递归版与显式栈版（模拟递归）不一致');
    if (naiveDFS(g.adj, n, s).join() !== exp.join()) naiveDiff++;
    // ② 每点恰好一次
    if (new Set(F.order).size !== n || F.order.length !== n) bad(tag + ' 有重复/漏访问');
    // ③ 时间戳：2n 个互不相同
    const all = [];
    for (let u = 0; u < n; u++) all.push(F.tin[u], F.tout[u]);
    if (all.some(x => x <= 0)) bad(tag + ' 有时间戳为 0');
    if (new Set(all).size !== 2 * n) bad(tag + ' 时间戳重复');
    if (Math.max(...all) !== 2 * n) bad(tag + ' 最大时间戳应为 ' + 2 * n + '，实际 ' + Math.max(...all));
    // ④ 状态与时间戳自洽 + 栈是一条链
    for (const f of frames) {
      for (let u = 0; u < n; u++) {
        if (f.state[u] === 2 && f.tout[u] === 0) bad(tag + ' 已完成但 tout=0');
        if (f.state[u] === 1 && f.tout[u] !== 0) bad(tag + ' 在栈上却已有 tout');
        if (f.state[u] === 0 && (f.tin[u] || f.tout[u])) bad(tag + ' 未访问却有时间戳');
        if (f.state[u] === 1 && !f.stack.includes(u)) bad(tag + ' 状态橙但不在栈里');
      }
      for (let i = 1; i < f.stack.length; i++) {
        const a = f.stack[i - 1], b = f.stack[i];
        if (!(f.tin[a] < f.tin[b])) bad(tag + ' 栈不是一条链');
        if (f.tout[b] !== 0 && f.tout[b] > f.tout[a]) bad(tag + ' 栈内区间关系错');
      }
    }
    // ⑤ 入/出栈配对
    let depth = 0, maxDepth = 0;
    for (const f of frames) {
      if (f.kind === 'push') depth++;
      if (f.kind === 'pop') depth--;
      if (depth < 0) bad(tag + ' 负栈深');
      maxDepth = Math.max(maxDepth, depth);
      if (depth !== f.stack.length) bad(tag + ' 栈深与记录不一致');
    }
    if (depth !== 0) bad(tag + ' 结束后栈非空');
    // ⑥ 边分类自洽
    const seenEdge = new Set(), clsCount = {};
    for (const f of frames) {
      if (f.kind !== 'examine') continue;
      const { u, v, cls } = f;
      clsCount[cls] = (clsCount[cls] || 0) + 1;
      if (!g.adj[u].includes(v)) bad(tag + ' 考察了不存在的边 ' + u + '->' + v);
      const st = f.state[v];
      if (cls === 'tree' && st !== 0) bad(tag + ' 树边 ' + u + '->' + v + ' 但目标不是白色');
      if (cls === 'back' && st !== 1) bad(tag + ' 回边 ' + u + '->' + v + ' 但目标不在栈上');
      if (cls === 'forward' && st !== 2) bad(tag + ' 前向边目标未完成');
      if (cls === 'cross' && st !== 2) bad(tag + ' 横叉边目标未完成');
      if (cls === 'repeat' && g.directed) bad(tag + ' 有向图不该出现 repeat');
      seenEdge.add(u + '-' + v);
    }
    for (let u = 0; u < n; u++) for (const v of g.adj[u]) if (!seenEdge.has(u + '-' + v)) bad(tag + ' 漏考察边 ' + u + '->' + v);
    // 统计面板必须与逐帧分类完全一致
    const panel = F.cnt;
    for (const k of ['tree', 'back', 'forward', 'cross', 'repeat'])
      if ((panel[k] || 0) !== (clsCount[k] || 0)) bad(tag + ' 面板统计 ' + k + '=' + panel[k] + ' 与逐帧 ' + (clsCount[k] || 0) + ' 不符');
    // ⑦ 无向图不该有 forward/cross
    if (!g.directed && (clsCount.forward || clsCount.cross)) bad(tag + ' 无向图出现 forward/cross');
    // ⑧ 教学用有向图应当四种边齐全
    if (mode === 'directed' && s === 0)
      for (const need of ['tree', 'back', 'forward', 'cross']) if (!clsCount[need]) bad(tag + ' 缺 ' + need);
    // ⑨ 树边数 == n - 连通块数
    const comps = (() => { let c = 0; const seen = new Array(n).fill(false);
      for (let v = 0; v < n; v++) if (!seen[v]) { c++; const st = [v]; seen[v] = true;
        while (st.length) { const u = st.pop(); for (const w of g.adj[u]) if (!seen[w]) { seen[w] = true; st.push(w); } } }
      return c; })();
    if (clsCount.tree !== n - comps) bad(tag + ' 树边数 ' + clsCount.tree + ' ≠ n-连通块 ' + (n - comps));

    if (s === 0) {
      console.log(tag + ' frames=' + frames.length + ' maxStackDepth=' + maxDepth +
        ' order=' + F.order.join(' '));
      console.log('        tree=' + clsCount.tree + ' back=' + (clsCount.back || 0) +
        (g.directed ? ' forward=' + (clsCount.forward || 0) + ' cross=' + (clsCount.cross || 0)
                    : ' repeat=' + (clsCount.repeat || 0)) +
        ' | 连通块数=' + comps);
    }
  }
}

console.log('----');
console.log('检查 ' + groups + ' 组（模式 × 起点），合计 ' + totalFrames + ' 帧');
console.log('附注：朴素迭代 DFS（入栈即标记）在其中 ' + naiveDiff + ' 组给出与递归不同的访问顺序 —— 这是真实现象，见讲义');

// ---------- 界面层的静态交叉检查：JS 里引用的 id 是否都在 HTML 里存在 ----------
const ids = new Set([...html.matchAll(/\bid="([^"]+)"/g)].map(x => x[1]));
const used = new Set();
for (const mm of html.matchAll(/\$\('#([A-Za-z0-9_-]+)'\)/g)) used.add(mm[1]);
for (const mm of html.matchAll(/getElementById\('([A-Za-z0-9_-]+)'\)/g)) used.add(mm[1]);
const missing = [...used].filter(x => !ids.has(x) && !['e', 'n'].includes(x));
console.log('HTML 里的 id：' + [...ids].join(', '));
console.log('JS 引用的 id ：' + [...used].join(', '));
if (missing.length) bad('JS 引用了不存在的 id: ' + missing.join(', '));
// 动态创建的节点（#n0 / #e0）不在静态 HTML 里，改为检查 JS 确实按前缀拼出了它们
const code = m[1];
if (!/id:\s*'n'\s*\+/.test(code)) bad("JS 没有动态创建 'n'+u 的节点");
if (!/id:\s*'e'\s*\+/.test(code)) bad("JS 没有动态创建 'e'+i 的边");

console.log(fail === 0 ? '全部通过 OK' : 'FAIL ' + fail + ' 处');
process.exit(fail === 0 ? 0 : 1);
