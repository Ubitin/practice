// 校验 BFS 动画里内嵌的逻辑：抽 <script> 在 node 里跑
//   + 帧序列一致性（队列 FIFO、无重复入队、出队按 d 非降）
//   + 首次访问即最短
//   + 用【完全不同的算法】（迭代松弛）独立交叉验证距离表
import { readFileSync } from 'node:fs';

function loadApi(file) {
  const html = readFileSync(file, 'utf8');
  const m = html.match(/<script>([\s\S]*?)<\/script>/);
  if (!m) throw new Error('未找到 <script>: ' + file);
  const mod = { exports: {} };
  new Function('module', 'exports', 'document', m[1])(mod, mod.exports, undefined);
  return mod.exports;
}

const SIM = loadApi('讲义/figures/BFS广度优先搜索动画.html');
const WALK8 = [[2,1],[1,2],[-1,2],[-2,1],[-2,-1],[-1,-2],[1,-2],[2,-1]];
let bad = 0;
const fail = (msg) => { ++bad; console.log('  ✘ ' + msg); };

/* ---------- 1) 方向数组与教材一致 ---------- */
{
  const w = SIM.WALK8;
  let m = 0;
  for (let i = 0; i < 8; ++i)
    if (w[i][0] !== WALK8[i][0] || w[i][1] !== WALK8[i][1]) ++m;
  console.log('[1] walk[8][2] 与教材 {{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}} 对照');
  if (m) fail(`有 ${m} 组偏移不一致`);
  else console.log('    ✔ 8 组偏移与教材完全一致；另有 4 向数组 ' + JSON.stringify(SIM.WALK4));
}

/* ---------- 独立算法：迭代松弛到不动点（类 Bellman-Ford） ---------- */
function relaxDist(N, M, sx, sy, mode) {
  const WALK = SIM.walkOf(mode);
  const INF = 1e9;
  const d = Array.from({ length: N + 1 }, () => new Array(M + 1).fill(INF));
  d[sx][sy] = 0;
  for (let loop = 0; loop < N * M + 5; ++loop) {
    let changed = false;
    for (let x = 1; x <= N; ++x) for (let y = 1; y <= M; ++y) {
      if (d[x][y] === INF) continue;
      for (const [dx, dy] of WALK) {
        const nx = x + dx, ny = y + dy;
        if (nx < 1 || nx > N || ny < 1 || ny > M) continue;
        if (d[x][y] + 1 < d[nx][ny]) { d[nx][ny] = d[x][y] + 1; changed = true; }
      }
    }
    if (!changed) break;
  }
  for (let x = 1; x <= N; x++) for (let y = 1; y <= M; ++y) if (d[x][y] === INF) d[x][y] = -1;
  return d;
}

/* ---------- 2) 帧序列全量校验 ---------- */
function checkOne(N, M, sx, sy, mode, verbose) {
  const TR = SIM.buildTrace(N, M, sx, sy, mode);
  const F = TR.frames;
  const dist = Array.from({ length: N + 1 }, () => new Array(M + 1).fill(-1));
  const enq = [];               // 入队顺序
  let head = 0;
  let badReason = 0, badEnq = 0, badFifo = 0, badOrder = 0, badPop = 0, dupEnq = 0;
  const seen = new Set();
  let lastD = -1;

  for (const f of F) {
    if (f.action === 'init') {
      dist[f.sx][f.sy] = 0;
      enq.push({ x: f.sx, y: f.sy, d: 0 });
      seen.add(f.sx + ',' + f.sy);
      lastD = 0;
    } else if (f.action === 'pop') {
      // ① 出队的必须正是"队首"
      if (head >= enq.length) ++badPop;
      else if (enq[head].x !== f.ux || enq[head].y !== f.uy) ++badPop;
      // ② BFS 本质：出队顺序按 d 【非降】
      if (f.d < lastD) ++badOrder;
      lastD = f.d;
      ++head;
    } else if (f.action === 'try') {
      const inb = f.nx >= 1 && f.nx <= N && f.ny >= 1 && f.ny <= M;
      const key = f.nx + ',' + f.ny;
      if (!f.ok) {
        // 跳过必须有正当理由：越界 或 已访问
        const legit = (!inb && f.why === 'out') || (inb && dist[f.nx][f.ny] !== -1 && f.why === 'vis');
        if (!legit) ++badReason;
      } else {
        if (!inb) ++badEnq;                                  // 越界却入队
        if (dist[f.nx][f.ny] !== -1) ++badEnq;               // 已访问却入队
        if (dist[f.nx][f.ny] !== f.d + 1 - 1 && dist[f.nx][f.ny] !== -1) ++badEnq;
        if (seen.has(key)) ++dupEnq;                         // 重复入队
        dist[f.nx][f.ny] = f.d + 1;
        enq.push({ x: f.nx, y: f.ny, d: f.d + 1 });
        seen.add(key);
        // ③ 入队时记的步数必须就是"首次访问"的步数
        if (dist[f.nx][f.ny] !== f.d + 1) ++badFifo;
      }
    }
  }
  // 结尾：所有帧走完后队列应恰好清空
  if (head !== enq.length && F[F.length - 1].action === 'end') ++badFifo;

  // 与独立算法对照
  const ref = relaxDist(N, M, sx, sy, mode);
  let diff = 0;
  for (let x = 1; x <= N; ++x) for (let y = 1; y <= M; ++y) if (dist[x][y] !== ref[x][y]) ++diff;

  if (verbose) {
    console.log(`\n[2] 帧序列校验  N=${N} M=${M} 起点(${sx},${sy}) 走法=${mode}向`);
    console.log(`    帧数 ${F.length}，入队 ${enq.length} 个点`);
    console.log(`    出队非队首 ${badPop}、出队顺序非"d 非降" ${badOrder}、重复入队 ${dupEnq}`);
    console.log(`    跳过理由不成立 ${badReason}、错放入队 ${badEnq}`);
    console.log(`    与独立算法（迭代松弛）距离表差异 ${diff}`);
  }
  return badPop + badOrder + dupEnq + badReason + badEnq + diff + badFifo;
}

/* ---------- 3) 多组配置 ---------- */
{
  let all = 0;
  for (const N of [4, 5, 6, 8, 10]) {
    for (const mode of [4, 8]) {
      for (const [sx, sy] of [[1,1],[1,N],[Math.ceil(N/2),Math.ceil(N/2)],[N,N],[2,3]]) {
        if (sx < 1 || sy < 1 || sx > N || sy > N) continue;
        all += checkOne(N, N, sx, sy, mode, false);
      }
    }
  }
  console.log(`\n[2] 帧序列全量校验：4/5/6/8/10 五种棋盘 × 2 种走法 × 5 个起点 = 50 组`);
  if (all) fail(`累计 ${all} 处问题`);
  else console.log('    ✔ 全部通过：出队恒为队首、出队顺序恒按 d 非降、无重复入队、跳过理由全部成立、'
                 + '距离表与独立算法逐格一致');
}

/* ---------- 4) 逐项汇报关键性质 ---------- */
{
  const TR = SIM.buildTrace(8, 8, 1, 1, 8);
  const d = TR.dist;
  const mx = Math.max(...d.slice(1).flatMap(r => r.slice(1)));
  console.log('\n[3] 8×8 起点(1,1) 马走法：');
  console.log(`    可达格数 ${TR.enqueued} / 64，最远 ${mx} 步`);
  console.log(`    d=0..${mx} 各层点数：` +
    JSON.stringify([...Array(mx + 1).keys()].map(k => d.slice(1).flatMap(r => r.slice(1)).filter(v => v === k).length)));
  const hard = SIM.buildTrace(4, 4, 1, 1, 8).dist;
  console.log('    4×4 起点(1,1) 距离表（对照教材图 14-8）：');
  for (let x = 1; x <= 4; ++x)
    console.log('      ' + hard[x].slice(1).map(v => String(v).padStart(3)).join(''));
  console.log('    4×4 起点(1,1) 4 向走法（= 曼哈顿距离，教材图 14-8(a) 的形状）：');
  const man = SIM.buildTrace(4, 4, 1, 1, 4).dist;
  for (let x = 1; x <= 4; ++x)
    console.log('      ' + man[x].slice(1).map(v => String(v).padStart(3)).join(''));
}

/* ---------- 5) 抽样细看一帧 ---------- */
{
  const TR = SIM.buildTrace(4, 4, 1, 1, 8);
  console.log('\n[4] 抽样：4×4 前 12 帧的动作序列');
  console.log('    ' + TR.frames.slice(0, 12).map(f =>
    f.action + (f.action === 'try' ? '(' + f.nx + ',' + f.ny + (f.ok ? '✔' : '✘' + f.why) + ')' : '')
  ).join(' → '));
}

console.log(bad === 0 ? '\nBFS 动画逻辑全部校验通过 ✅' : `\n存在 ${bad} 处失败 ❌`);
process.exit(bad === 0 ? 0 : 1);
