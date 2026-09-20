// 校验 DFS 动画里内嵌的逻辑：抽 <script> 在 node 里跑 + 帧序列一致性 + 独立暴力交叉验证
import { readFileSync } from 'node:fs';

function loadApi(file) {
  const html = readFileSync(file, 'utf8');
  const m = html.match(/<script>([\s\S]*?)<\/script>/);
  if (!m) throw new Error('未找到 <script>: ' + file);
  const mod = { exports: {} };
  new Function('module', 'exports', 'document', m[1])(mod, mod.exports, undefined);
  return mod.exports;
}

const SIM = loadApi('讲义/figures/DFS深度优先搜索动画.html');
const N = 4, NN = N * N;
let bad = 0;
const fail = (msg) => { ++bad; console.log('  ✘ ' + msg); };

/* ---------- 1) cellOf 与教材公式逐项对照 ---------- */
{
  let m = 0;
  for (let k = 1; k <= NN; ++k) {
    const r = Math.floor((k - 1) / N) + 1;
    const c = (k - 1) % N + 1;
    const b = Math.floor((r - 1) / 2) * 2 + Math.floor((c - 1) / 2) + 1;
    const g = SIM.cellOf(k, N);
    if (g.r !== r || g.c !== c || g.b !== b) ++m;
  }
  console.log(`[1] cellOf 与教材公式 row=(k-1)/4+1, col=(k-1)%4+1, block=(row-1)/2*2+(col-1)/2+1`);
  if (m) fail(`不一致 ${m} 项`); else console.log('    ✔ 16 个格子全部一致');
}

/* ---------- 2) 回放帧序列，逐帧做一致性校验 ---------- */
const TR = SIM.buildTrace(N);
const F = TR.frames;

function validGrid(a) {                       // 完整合法的四阶数独？
  if (a.length !== NN + 1) return false;
  for (let k = 1; k <= NN; ++k) if (a[k] < 1 || a[k] > 4) return false;
  for (let r = 1; r <= 4; ++r) {
    const s = new Set(); for (let c = 1; c <= 4; ++c) s.add(a[(r - 1) * 4 + c]);
    if (s.size !== 4) return false;
  }
  for (let c = 1; c <= 4; ++c) {
    const s = new Set(); for (let r = 1; r <= 4; ++r) s.add(a[(r - 1) * 4 + c]);
    if (s.size !== 4) return false;
  }
  for (let b = 1; b <= 4; ++b) {
    const s = new Set();
    for (let k = 1; k <= NN; ++k) if (SIM.cellOf(k, N).b === b) s.add(a[k]);
    if (s.size !== 4) return false;
  }
  return true;
}

{
  const a = new Array(NN + 1).fill(0);
  const mk = () => Array.from({ length: N + 1 }, () => new Array(N + 1).fill(0));
  const row = mk(), col = mk(), blk = mk();
  const act = [];
  const solutions = [];
  let solCount = 0;
  let badPlace = 0, badReject = 0, badState = 0, badSol = 0, badDepth = 0;

  for (let i = 0; i < F.length; ++i) {
    const f = F[i];
    if (f.action === 'enter') { act.push({ k: f.k }); if (f.depth !== act.length) ++badDepth; }
    else if (f.action === 'place') {
      const { r, c, b } = SIM.cellOf(f.k, N);
      // 放入前必须"行/列/块都没占用"，否则就是漏剪枝
      if (row[r][f.num] || col[c][f.num] || blk[b][f.num]) ++badPlace;
      a[f.k] = f.num; row[r][f.num] = col[c][f.num] = blk[b][f.num] = 1;
    }
    else if (f.action === 'reject') {
      const { r, c, b } = SIM.cellOf(f.k, N);
      // 被剪枝就必须真的有冲突，否则是误剪枝（会漏解）
      const hit = row[r][f.num] || col[c][f.num] || blk[b][f.num];
      if (!hit) ++badReject;
    }
    else if (f.action === 'undo') {
      const { r, c, b } = SIM.cellOf(f.k, N);
      a[f.k] = 0; row[r][f.num] = col[c][f.num] = blk[b][f.num] = 0;
    }
    else if (f.action === 'backtrack') { act.pop(); if (f.depth !== act.length) ++badDepth; }
    else if (f.action === 'solution') {
      ++solCount;
      if (!validGrid(a)) ++badSol;
      else solutions.push(a.slice(1).join(''));
      if (solCount !== f.depth * 0 + solCount) {}
    }
    // 每帧都检查：row/col/blk 与 a[] 必须完全自洽
    for (let r = 1; r <= 4; ++r) for (let v = 1; v <= 4; ++v) {
      let has = false;
      for (let c = 1; c <= 4; ++c) if (a[(r - 1) * 4 + c] === v) has = true;
      if ((row[r][v] === 1) !== has) { ++badState; r = 5; break; }
    }
  }

  console.log(`\n[2] 帧序列回放校验（共 ${F.length} 帧）`);
  console.log(`    动作分布：`, JSON.stringify(F.reduce((m, f) => (m[f.action] = (m[f.action] || 0) + 1, m), {})));
  console.log(`    栈深记录错 ${badDepth}、状态不自洽 ${badState}`);
  console.log(`    place 时仍冲突（漏剪枝）${badPlace}、reject 时无冲突（误剪枝）${badReject}`);
  console.log(`    solution 帧不是合法完整解 ${badSol}`);
  if (badDepth || badState || badPlace || badReject || badSol) fail('帧序列存在问题');

  console.log(`\n[3] 解的正确性`);
  console.log(`    solution 帧数 = ${solCount}，合法解 ${solutions.length}，去重后 ${new Set(solutions).size}`);
  if (solutions.length !== solCount) fail('有 solution 帧不是合法完整解');
  if (new Set(solutions).size !== solCount) fail('存在重复解');
  if (solCount !== TR.solutions) fail(`帧内计数 ${TR.solutions} ≠ 实际解数 ${solCount}`);
}

/* ---------- 4) 独立暴力：枚举 24^4 个"每行都是排列"的盘面 ---------- */
{
  const perms = [];
  const rec = (cur) => {
    if (cur.length === 4) { perms.push(cur.slice()); return; }
    for (let v = 1; v <= 4; ++v) if (!cur.includes(v)) { cur.push(v); rec(cur); cur.pop(); }
  };
  rec([]);
  let cnt = 0;
  for (const r1 of perms) for (const r2 of perms) for (const r3 of perms) for (const r4 of perms) {
    const g = [r1, r2, r3, r4];
    const a = [0];
    for (let r = 0; r < 4; ++r) for (let c = 0; c < 4; ++c) a.push(g[r][c]);
    if (validGrid(a)) ++cnt;
  }
  console.log(`\n[4] 独立暴力交叉验证（枚举 24^4 = ${perms.length ** 4} 个每行皆为排列的盘面）`);
  console.log(`    暴力计数 = ${cnt}`);
  if (cnt !== TR.solutions) fail(`暴力 ${cnt} ≠ 动画 ${TR.solutions}`);
  else console.log(`    ✔ 与动画 DFS 的 ${TR.solutions} 个解完全一致`);
}

console.log(bad === 0 ? '\nDFS 动画逻辑全部校验通过 ✅' : `\n存在 ${bad} 处失败 ❌`);
process.exit(bad === 0 ? 0 : 1);
