import fs from 'fs';
const html = fs.readFileSync(process.argv[2], 'utf8');
const src = html.match(/<script>([\s\S]*?)<\/script>/)[1];
const module = { exports: {} };
new Function('module', 'exports', 'document', src)(module, module.exports, undefined);
const SIM = module.exports;

function rec(adj, n, first) {
  const seen = new Array(n).fill(false), order = [];
  const go = u => { seen[u] = true; order.push(u); for (const v of adj[u]) if (!seen[v]) go(v); };
  if (first >= 0) go(first);
  for (let v = 0; v < n; v++) if (!seen[v]) go(v);
  return order;
}
function naive(adj, n, first) {
  const seen = new Array(n).fill(false), order = [];
  const run = s => {
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
let diff = 0, tot = 0;
for (const mode of Object.keys(SIM.GRAPHS)) {
  const g = SIM.GRAPHS[mode];
  for (let s = 0; s < g.n; s++) {
    tot++;
    const a = rec(g.adj, g.n, s).join(' '), b = naive(g.adj, g.n, s).join(' ');
    if (a !== b) { diff++; console.log(`${mode} start=${s}: 递归=[${a}]  朴素迭代=[${b}]  <-- 不同`); }
  }
}
console.log(`共 ${tot} 组，朴素迭代与递归不同 ${diff} 组`);
