import fs from 'fs';
const html = fs.readFileSync(process.argv[2], 'utf8');
const src = html.match(/<script>([\s\S]*?)<\/script>/)[1];
const module = { exports: {} };
new Function('module', 'exports', 'document', src)(module, module.exports, undefined);
const SIM = module.exports;

const mode = process.argv[3] || 'undirected';
const start = parseInt(process.argv[4] || '0', 10);
const { frames } = SIM.run(mode, start);
console.log('mode=' + mode + ' start=' + start + ' frames=' + frames.length);
let n = 0;
for (const [i, f] of frames.entries()) {
  if (f.kind === 'examine') {
    n++;
    console.log('  f' + String(i).padStart(3) + ' examine ' + f.u + '->' + f.v + '  ' +
      f.cls.padEnd(8) + ' state[v]=' + f.state[f.v] + ' (white0/grey1/black2)');
  } else {
    console.log('  f' + String(i).padStart(3) + ' ' + String(f.kind || 'init').padEnd(8) + ' u=' + f.u);
  }
}
console.log('examined edges = ' + n);
console.log('final cnt = ' + JSON.stringify(frames[frames.length - 1].cnt));
const t = {};
for (const f of frames) if (f.kind === 'examine') t[f.cls] = (t[f.cls] || 0) + 1;
console.log('counted from frames = ' + JSON.stringify(t));
