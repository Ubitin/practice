// 校验两个网页动画里内嵌的纯逻辑（把 <script> 抽出来在 node 里跑 + 随机对拍）
import { readFileSync } from 'node:fs';

function loadApi(file) {
  const html = readFileSync(file, 'utf8');
  const m = html.match(/<script>([\s\S]*?)<\/script>/);
  if (!m) throw new Error('未找到 <script>: ' + file);
  const mod = { exports: {} };
  new Function('module', 'exports', 'document', m[1])(mod, mod.exports, undefined);
  return mod.exports;
}

let allOk = true;
const rnd = (k) => Math.floor(Math.random() * k);

/* ---------- 1) P1164：0/1 背包计数 ---------- */
{
  const SIM = loadApi('讲义/figures/P1164-背包计数动画.html');
  const P = [1, 1, 2, 2];
  const desc = SIM.buildSteps('desc', 4, 4, P), asc = SIM.buildSteps('asc', 4, 4, P);
  const brute = (n, m, a) => {
    let c = 0;
    for (let mask = 0; mask < (1 << n); ++mask) {
      let s = 0;
      for (let i = 0; i < n; ++i) if ((mask >> i) & 1) s += a[i];
      if (s === m) ++c;
    }
    return c;
  };
  let bad = 0;
  for (let t = 0; t < 500; ++t) {
    const n = 1 + rnd(8), m = 1 + rnd(14);
    const a = Array.from({ length: n }, () => 1 + rnd(6));
    if (SIM.buildSteps('desc', n, m, a).final[m] !== brute(n, m, a)) ++bad;
  }
  const ok = desc.final[4] === 3 && asc.final[4] === 14 && bad === 0;
  allOk = allOk && ok;
  console.log(`[P1164 背包计数] 样例 倒序=${desc.final[4]} 正序=${asc.final[4]} | `
            + `随机 500 组错误 ${bad} → ${ok ? '通过 ✅' : '失败 ❌'}`);
}

/* ---------- 2) CF1805C：最近邻二分 ---------- */
{
  const SIM = loadApi('讲义/figures/CF1805C-最近邻二分动画.html');
  const refLB = (ks, b) => { let i = 0; while (i < ks.length && ks[i] < b) ++i; return i; };

  let bad = 0, badPos = 0, badFrames = 0;
  for (let t = 0; t < 800; ++t) {
    const n = 1 + rnd(10);
    const ks = Array.from({ length: n }, () => rnd(61) - 30).sort((x, y) => x - y);
    const paras = Array.from({ length: 1 + rnd(5) },
      () => ({ a: 1 + rnd(20), b: rnd(81) - 40, c: 1 + rnd(20) }));

    const { frames, results } = SIM.buildFrames(ks, paras);
    results.forEach((r, idx) => {
      const brute = ks.some(k => (r.b - k) * (r.b - k) < 4 * r.a * r.c);
      if (r.verdict !== brute) ++bad;
      if (r.pos !== refLB(ks, r.b)) ++badPos;
      if (r.verdict && !ks.includes(r.chosen)) ++bad;
      // 帧的一致性：用帧里的 goLeft 回放，应能还原出 pos，且每帧窗口都包含 pos
      const searchFrames = frames.filter(f => f.pi === idx && f.phase === 'search');
      let lo = 0, hi = ks.length;
      for (const f of searchFrames) {
        if (!(f.lo === lo && f.hi === hi)) ++badFrames;          // 帧记录的区间要连续
        if (!(f.lo <= r.pos && r.pos <= f.hi)) ++badFrames;      // 不变式：pos 始终在窗口内
        if (f.goLeft !== (ks[f.mid] >= r.b)) ++badFrames;
        if (f.goLeft) hi = f.mid; else lo = f.mid + 1;
      }
      if (lo !== r.pos || lo !== hi) ++badFrames;
      const want = [];
      if (r.pos < ks.length) want.push(r.pos);
      if (r.pos - 1 >= 0) want.push(r.pos - 1);
      if (JSON.stringify(r.cands) !== JSON.stringify(want)) ++badFrames;
    });
  }
  const ok = bad === 0 && badPos === 0 && badFrames === 0;
  allOk = allOk && ok;
  console.log(`[CF1805C 最近邻二分] 随机 800 组：判定错 ${bad}、pos 错 ${badPos}、帧结构错 ${badFrames}`
            + ` → ${ok ? '通过 ✅' : '失败 ❌'}`);
}

console.log(allOk ? '两个动画逻辑全部校验通过 ✅' : '存在失败项 ❌');
process.exit(allOk ? 0 : 1);
