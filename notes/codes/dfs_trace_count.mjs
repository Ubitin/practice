// 统计四阶数独 DFS 的完整轨迹规模，决定动画怎么设计
function buildTrace(N) {
  const B = 2;                                  // 2x2 小方块
  const a = new Array(N * N + 1).fill(0);
  const mk = () => Array.from({ length: N + 1 }, () => new Array(N + 1).fill(0));
  const row = mk(), col = mk(), blk = mk();
  const frames = [];
  const stack = [];
  let solutions = 0;
  let firstSolutionAt = -1, firstBacktrackAt = -1;

  const cell = (k) => {
    const r = Math.floor((k - 1) / N) + 1;
    const c = (k - 1) % N + 1;
    const b = Math.floor((r - 1) / B) * B + Math.floor((c - 1) / B) + 1;
    return { r, c, b };
  };
  const snap = (action, k, num, reason) => {
    frames.push({ action, k, num: num || 0, reason: reason || '',
                  a: a.slice(), solutions,
                  depth: stack.length,
                  stack: stack.map(s => ({ k: s.k, tried: s.tried })) });
  };

  function dfs(k) {
    if (k > N * N) {
      solutions++;
      snap('solution', k, 0, '');
      if (firstSolutionAt < 0) firstSolutionAt = frames.length - 1;
      return;
    }
    stack.push({ k, tried: 0 });
    snap('enter', k, 0, '');
    const { r, c, b } = cell(k);
    for (let num = 1; num <= N; ++num) {
      stack[stack.length - 1].tried = num;
      if (row[r][num] || col[c][num] || blk[b][num]) {
        snap('reject', k, num, row[r][num] ? 'row' : (col[c][num] ? 'col' : 'blk'));
        continue;
      }
      a[k] = num; row[r][num] = col[c][num] = blk[b][num] = 1;
      snap('place', k, num, '');
      dfs(k + 1);
      a[k] = 0; row[r][num] = col[c][num] = blk[b][num] = 0;
      snap('undo', k, num, '');
    }
    stack.pop();
    snap('backtrack', k, 0, '');
    if (firstBacktrackAt < 0) firstBacktrackAt = frames.length - 1;
  }
  dfs(1);
  return { frames, solutions, firstSolutionAt, firstBacktrackAt };
}

for (const N of [4]) {
  const t0 = Date.now();
  const r = buildTrace(N);
  const byAction = {};
  for (const f of r.frames) byAction[f.action] = (byAction[f.action] || 0) + 1;
  console.log(`N=${N}`);
  console.log(`  总帧数   = ${r.frames.length}`);
  console.log(`  解的个数 = ${r.solutions}`);
  console.log(`  第一个解在第 ${r.firstSolutionAt} 帧`);
  console.log(`  第一次回溯在第 ${r.firstBacktrackAt} 帧`);
  console.log(`  各类动作：`, JSON.stringify(byAction));
  console.log(`  耗时 ${Date.now() - t0} ms`);
  // 最大栈深
  let mx = 0; for (const f of r.frames) mx = Math.max(mx, f.depth);
  console.log(`  最大栈深 = ${mx}`);
}
