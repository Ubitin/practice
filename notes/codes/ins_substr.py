# -*- coding: utf-8 -*-
# 在手册 §2.3 之前插入「substr 返回值必须接住」专项小节
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("### 2.3 pair"))
print("anchor L%d: %s" % (i + 1, lines[i]))

Q = chr(34)
SQ = chr(39)
blk = [
 '### 2.2.1 substr 的返回值必须接住（「写了但没生效」类 bug，实测实录）',
 '```cpp',
 '// 场景：输入两个形如 a/b 的分数，要把分子、分母分别取出来',
 'string s = ' + Q + '1/2' + Q + ';',
 'll pos = s.find(' + SQ + '/' + SQ + ');',
 '',
 '// 错：substr 不会修改原字符串，它返回一个新串；不接住就等于什么都没做',
 'string son = s, mom = s;',
 'son.substr(0, pos);  mom.substr(pos + 1);',
 '//   实测：son 仍然是 1/2、mom 仍然是 1/2',
 '//   而 stoi(1/2) 遇到斜杠就停下，返回 1 ⇒ 分母丢了，两个分数都变成 1/1',
 '//   编译器会警告（-Wall 里就有）：',
 '//     warning: ignoring return value of ...substr... [-Wunused-result]',
 '//     ★ 这条警告不是噪音，是「你的代码没生效」的直接提示',
 '',
 '// 对：把返回值赋回去',
 'string son2 = s.substr(0, pos);      // 得到 1',
 'string mom2 = s.substr(pos + 1);     // 得到 2',
 '// 或者用 stoll 直接取，更省事：',
 'll a = stoll(s.substr(0, pos)), b = stoll(s.substr(pos + 1));',
 '```',
 '// 同族陷阱（看起来像原地修改、其实返回新对象的 STL 成员）：',
 '//   substr、to_string、string::append 的返回值、vector::erase 的返回值（返回下一个迭代器）',
 '//   判据：签名带 const 且返回非 void 的成员函数，基本都不会修改自己',
 '//',
 '// 这题同时暴露的另外 4 个坑（都实测过）：',
 '//   1) if(!x % i) 想表达 x 能被 i 整除，实际是 (!x) % i；必须写 x % i == 0',
 '//   2) 化简分数的循环从 min(x,y) 起手：x 或 y 为负数时循环直接不执行 ⇒ 负数永不化简',
 '//      ⇒ 用标准 gcd：while(b){t=a%b;a=b;b=t;}，并先对 a、b 取绝对值',
 '//   3) 分子为 0（如 0/5）要特判输出 0/1，否则会输出 0/5 或 0/0',
 '//   4) 除法运算可能让分母变成 0（除数的分子为 0）⇒ 提前判断并处理',
 '// 实测（test_\\01_fraction_fixed.cpp vs Python fractions.Fraction）：49 组含负数/0/大数，不一致 0',
 '',
]
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("inserted, total lines =", len(lines))
