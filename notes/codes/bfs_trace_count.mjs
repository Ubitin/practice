// 统计骑士 BFS 的轨迹规模 + 用「完全不同的算法」独立算出距离表
const WALK = [[2,1],[1,2],[-1,2],[-2,1],[-2,-1],[-1,-2],[1,-2],[2,-1]];

function bfsDist(N, M, sx, sy) {                 // 标准 BFS
  const d = Array.from({length:N+1},()=>new Array(M+1).fill(-1));
  d[sx][sy] = 0;
  const q = [[sx,sy]];
  for (let h = 0; h < q.length; ++h) {
    const [x,y] = q[h];
    for (const [dx,dy] of WALK) {
      const nx = x+dx, ny = y+dy;
      if (nx<1||nx>N||ny<1||ny>M) continue;
      if (d[nx][ny] !== -1) continue;
      d[nx][ny] = d[x][y] + 1;
      q.push([nx,ny]);
    }
  }
  return d;
}

function relaxDist(N, M, sx, sy) {               // 独立算法：迭代松弛到不动点（类 Bellman-Ford）
  const INF = 1e9;
  const d = Array.from({length:N+1},()=>new Array(M+1).fill(INF));
  d[sx][sy] = 0;
  let changed = true, round = 0;
  while (changed) {
    changed = false; ++round;
    for (let x = 1; x <= N; ++x) for (let y = 1; y <= M; ++y) {
      if (d[x][y] === INF) continue;
      for (const [dx,dy] of WALK) {
        const nx = x+dx, ny = y+dy;
        if (nx<1||nx>N||ny<1||ny>M) continue;
        if (d[x][y] + 1 < d[nx][ny]) { d[nx][ny] = d[x][y] + 1; changed = true; }
      }
    }
  }
  for (let x = 1; x <= N; ++x) for (let y = 1; y <= M; ++y)
    if (d[x][y] === INF) d[x][y] = -1;
  return { d, round };
}

// 轨迹规模（与动画里 buildTrace 同构的计数）
function traceStats(N, M, sx, sy) {
  const dist = Array.from({length:N+1},()=>new Array(M+1).fill(-1));
  dist[sx][sy] = 0;
  const q = [{x:sx,y:sy,d:0}];
  let frames = 1, head = 0, push = 0, skip = 0, pop = 0;
  while (head < q.length) {
    const u = q[head++]; ++pop; frames += 2;      // pop + done
    for (let k = 0; k < 8; ++k) {
      const nx = u.x + WALK[k][0], ny = u.y + WALK[k][1];
      if (nx<1||nx>N||ny<1||ny>M || dist[nx][ny] !== -1) { ++skip; }
      else { dist[nx][ny] = u.d+1; q.push({x:nx,y:ny,d:u.d+1}); ++push; }
      frames++;
    }
  }
  frames++;                                       // end
  const layers = {};
  for (const u of q) layers[u.d] = (layers[u.d]||0)+1;
  return { frames, pop, push, skip, layers, enqueued: q.length };
}

for (const [N,M,sx,sy] of [[4,4,1,1],[6,6,1,1],[8,8,1,1]]) {
  const s = traceStats(N,M,sx,sy);
  const a = bfsDist(N,M,sx,sy), b = relaxDist(N,M,sx,sy).d;
  let diff = 0;
  for (let x=1;x<=N;++x) for (let y=1;y<=M;++y) if (a[x][y]!==b[x][y]) ++diff;
  console.log(`棋盘 ${N}x${M}，起点 (${sx},${sy})`);
  console.log(`  帧数 ${s.frames}（pop ${s.pop} / push ${s.push} / skip ${s.skip} / end 1）`);
  console.log(`  各层点数：`, JSON.stringify(s.layers));
  console.log(`  BFS 与"迭代松弛"两种算法距离表差异：${diff}  → ${diff===0?'一致 ✔':'不一致 ✘'}`);
  console.log(`  最远步数 = ${Math.max(...Object.keys(s.layers).map(Number))}，可达格 ${s.enqueued}/${N*M}`);
}
// 打印 4x4 的距离表（与教材图 14-8 对照）
console.log('\n4x4 起点(1,1) 距离表：');
const d = bfsDist(4,4,1,1);
for (let x=1;x<=4;++x) console.log('  ' + d[x].slice(1).map(v=>String(v).padStart(3)).join(''));
