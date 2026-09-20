// 从 HTML 动画页面里抽出算法部分（buildSteps）并验证输出，确保"演示的就是正确算法"
const fs = require('fs');
const path = 'D:/lenovo/chat_with_deepseek_harness/讲义/figures/P1928-展开动画.html';
const html = fs.readFileSync(path, 'utf8');

const m = html.match(/<script>([\s\S]*?)<\/script>/);
if (!m) { console.log('未找到 script 块'); process.exit(1); }
let code = m[1];
// 只保留第 1 段（算法），去掉依赖 DOM 的渲染/控制代码
code = code.split('// ============ 2. 渲染')[0];
code = code.replace(/^const \$ = .*$/m, '');
eval(code);                       // 定义 buildSteps

const cases = [
  ['AC[3FUN]',            'ACFUNFUNFUN'],
  ['ABF[4RA[2A]B[3C]]',   'ABF' + 'RAAABCCC'.repeat(4)],
  ['[2[2[2AB]]]',         'AB'.repeat(8)],
  ['[12XY]',              'XY'.repeat(12)],
  ['[2A]B[3C]',           'AABCCC'],
  ['ABCDEF',              'ABCDEF'],
  ['[3CBCB]',             'CBCB'.repeat(3)],
  ['[2[2CB]]',            'CBCBCBCB'],
  ['[99Z]',               'Z'.repeat(99)],
];

let bad = 0;
for (const [inp, exp] of cases) {
  const { steps, result } = buildSteps(inp);
  const ok = result === exp;
  if (!ok) bad++;
  console.log(`${ok ? '✓' : '✗'} ${inp.padEnd(20)} => ${result.padEnd(30)} ${ok ? '' : '期望 ' + exp}  (步数 ${steps.length})`);
}
console.log(bad === 0 ? '\n全部通过：动画所用算法与预期输出一致' : `\n有 ${bad} 个不一致`);
