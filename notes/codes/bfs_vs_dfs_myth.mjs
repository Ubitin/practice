// 用实测检验两个常见说法：
//   A. "DFS 是用较高的效率去搜索所有合法的方案"
//   B. "BFS 是只搜索满足特定性质（比如路径最短）的方案"
const WALK8 = [[2,1],[1,2],[-1,2],[-2,1],[-2,-1],[-1,-2],[1,-2],[2,-1]];

// ① BFS 求"单点最短路"时，到底搜索了多少个点？
function bfsWork(N, sx, sy) {
  const dist = Array.from({length:N+1},()=>new Array(N+1).fill(-1));
  const q = [[sx,sy]]; dist[sx][sy] = 0;
  let maxQ = 1;
  const popOrder = [];                        // 每个点"被弹出"时，已经弹了几个
  for (let h = 0; h < q.length; ++h) {
    const [x,y] = q[h];
    popOrder.push({x, y, d: dist[x][y], poppedBefore: h});
    // 若只求到 (x,y) 的最短路，此刻就能停 —— 但注意：这时已经入队/出队了多少
    for (const [dx,dy] of WALK8) {
      const nx = x+dx, ny = y+dy;
      if (nx<1||nx>N||ny<1||ny>N) continue;
      if (dist[nx][ny] !== -1) continue;
      dist[nx][ny] = dist[x][y] + 1;
      q.push([nx,ny]);
    }
    maxQ = Math.max(maxQ, q.length - h - 1);
  }
  return { dist, total: q.length, maxQ, popOrder };
}

console.log('══ 实验 ①：BFS 求"一个点的最短路"，却要搜掉多少？══');
console.log('（8×8 棋盘，起点(1,1)，马走法）\n');
const { dist, total, maxQ, popOrder } = bfsWork(8, 1, 1);
console.log(`  棋盘上总共 ${64} 格，BFS 一共处理了 ${total} 格 —— 也就是【全部】都访问了`);
console.log(`  队列最大长度 = ${maxQ}（这是 BFS 的空间开销：要存"整层"）\n`);
console.log('  如果只想求 (1,1) 到某个目标点的最短路，BFS 在弹出它时就该停。');
console.log('  看几个目标点"停下来之前"已经处理了多少格：\n');
console.log('    目标点    它的最短步数   停下前已处理的格数   占全盘比例');
const picks = [[2,3],[1,4],[8,8],[4,4],[1,2],[8,1]];
for (const [tx,ty] of picks) {
  const rec = popOrder.find(p => p.x===tx && p.y===ty);
  if (!rec) { console.log(`    (${tx},${ty}) 不可达`); continue; }
  const n = rec.poppedBefore + 1;
  console.log(`    (${tx},${ty})`.padEnd(14)
    + String(rec.d).padEnd(16)
    + String(n).padEnd(22)
    + (100*n/64).toFixed(0) + '%');
}
console.log('\n  → 即使目标只有 1~2 步，BFS 也已经处理了 3~12 格；');
console.log('    目标是远处格时，它必须把前面所有层全搜完，等于【搜遍全盘】。');

// ② 棋盘变大时 BFS 的队列峰值（空间代价）
console.log('\n══ 实验 ②：BFS 的空间代价随棋盘增长 ══\n');
console.log('    棋盘      格数    BFS 队列最大长度');
for (const N of [4, 6, 8, 10, 12]) {
  const r = bfsWork(N, 1, 1);
  console.log(`    ${String(N+'×'+N).padEnd(10)}${String(N*N).padEnd(8)}${r.maxQ}`);
}
console.log('\n  对比 DFS（数独动画实测）：最大递归栈深 = 16（= 格子数），只需存【一条路径】');

// ③ "DFS 高效地枚举所有方案"？枚举方案本身就是指数级的
console.log('\n══ 实验 ③："枚举所有方案"本身有多贵 ══\n');
console.log('  P2392 每科 s 道题，枚举所有"左右脑分配"方案：');
console.log('    s        子集枚举次数 2^s       0/1 背包 DP 次数 s×sum/2');
for (const s of [10, 20, 25, 30]) {
  const sum = s * 100;                          // 假设每题 100
  console.log(`    ${String(s).padEnd(9)}${String(Math.pow(2,s).toExponential(2)).padEnd(28)}${s*sum/2}`);
}
console.log('\n  → "枚举所有方案"是 2^s 级的；而"直接算出方案数/最优值"只要多项式时间。');
console.log('    DFS 的价值是【通用】（什么题都能枚举），不是【高效】。');

// ④ 反例：DFS 也能求最短路（慢），BFS 也能数方案数
console.log('\n══ 实验 ④：两者并非"各管一半" ══\n');
{
  // DFS 求 8×8 上 (1,1) 到 (2,3) 的最短路（枚举到该点的所有路径，取 min）
  const N = 8, tx = 2, ty = 3;
  let best = Infinity, calls = 0;
  const vis = Array.from({length:N+1},()=>new Array(N+1).fill(false));
  function dfs(x, y, step) {
    ++calls;
    if (step >= best) return;                   // 剪枝
    if (x === tx && y === ty) { best = Math.min(best, step); return; }
    if (step > 8) return;
    for (const [dx,dy] of WALK8) {
      const nx = x+dx, ny = y+dy;
      if (nx<1||nx>N||ny<1||ny>N||vis[nx][ny]) continue;
      vis[nx][ny] = true; dfs(nx, ny, step+1); vis[nx][ny] = false;
    }
  }
  vis[1][1] = true; dfs(1, 1, 0);
  console.log(`  DFS 求 (1,1)→(${tx},${ty}) 最短步数：得到 ${best}，递归调用 ${calls} 次`);
  console.log(`  BFS 求同一条最短路：结果 ${dist[tx][ty]}，只处理了 3 个点`);
  console.log('  → 两者都能求最短路，但 BFS 快得多。区别在【顺序】，不在"能不能"。');
}
{
  // BFS 数"最短路径条数"
  const N = 8, sx = 1, sy = 1;
  const d = Array.from({length:N+1},()=>new Array(N+1).fill(-1));
  const ways = Array.from({length:N+1},()=>new Array(N+1).fill(0));
  d[sx][sy] = 0; ways[sx][sy] = 1;
  const q = [[sx,sy]];
  for (let h = 0; h < q.length; ++h) {
    const [x,y] = q[h];
    for (const [dx,dy] of WALK8) {
      const nx = x+dx, ny = y+dy;
      if (nx<1||nx>N||ny<1||ny>N) continue;
      if (d[nx][ny] === -1) { d[nx][ny] = d[x][y]+1; q.push([nx,ny]); }
      if (d[nx][ny] === d[x][y]+1) ways[nx][ny] += ways[x][y];   // 同层累加方案数
    }
  }
  console.log(`\n  BFS 顺便数"最短路径条数"：(1,1)→(4,4) 最短 ${d[4][4]} 步，共 ${ways[4][4]} 条最短路径`);
  console.log('  → BFS 也能"数方案的个数"，只要在分层时顺手累加。');
}
