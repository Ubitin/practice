# -*- coding: utf-8 -*-
# 手册 §9 追加「9.4 程序没有任何输出：先看退出码」小节
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("## 10. lambda"))
print("anchor L%d: %s" % (i + 1, lines[i]))

blk = [
 '### 9.4 程序"一行输出都没有"怎么办：先看退出码，别先怀疑逻辑',
 '```text',
 '【首要判断】输出为空 + 退出码非 0  ⇒  进程崩了（不是逻辑错，逻辑错会打印点别的东西）',
 '【首要判断】输出为空 + 退出码为 0  ⇒  才是逻辑/读入问题（例如输入格式不对、条件全不成立）',
 '',
 '【Windows 退出码 → 病因对照（$LASTEXITCODE 看到负数就 +2^32 看十六进制）】',
 '  0xC0000005  ACCESS_VIOLATION    空指针/数组越界读写（隔三差五就要查）',
 '  0xC00000FD  STACK_OVERFLOW      递归太深 / 大数组开在函数内',
 '  0xC0000094  INT_DIVIDE_BY_ZERO  整数除零（分母没判 0）',
 '  0xC000001D  ILLEGAL_INSTRUCTION 未捕获的 C++ 异常 ⇒ fail-fast abort（★ 本题：stoll(空串)）',
 '  0xC0000374  HEAP_CORRUPTION     比较器违反严格弱序 / 越界写堆',
 '  0xC0000409  STACK_BUFFER_OVERRUN 缓冲区越界（fail-fast）',
 '',
 '【★ 最容易被忽略的一条：崩溃时"已打印的内容也会消失"】',
 '  cout 是带缓冲的，abort() 不会刷缓冲 ⇒ 屏上空白不代表"没执行到输出"',
 '  排查手段：① 用 cerr（无缓冲）打点  ② cout << flush  ③ 直接用 printf',
 '',
 '【本题实例（后缀表达式求值，用 @ 输出结果）】',
 '  ✘ string temp; 写在 for 循环【内部】⇒ 每轮重建为空串',
 '     ⇒ 遇到数字分隔符时 temp 是空串 ⇒ stoll("") 抛 std::invalid_argument ⇒ 无 try/catch ⇒ abort',
 '     实测：输入 9.@ / 12.@ / 5.3.+@ 全部"输出空 + 退出码 0xC000001D"；修正后 9 / 12 / 8',
 '  ✔ 修法：需要【跨轮累积】的状态必须声明在循环外面；stoll 前判空；或整段 try/catch',
 '',
 '【通用三条】',
 '  1. "没有输出"先看退出码，再怀疑逻辑',
 '  2. 循环内定义的变量 = 每轮重置；要累积的状态放循环外',
 '  3. stoll / stoi 要防御：空串、全非数字、超范围都会抛异常（atoi 是 UB，别用）',
 '```',
 ''
]
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))

fences = [j for j, l in enumerate(lines) if l.lstrip().startswith("```")]
inside = False
bad = 0
for l in lines:
    if l.lstrip().startswith("```"):
        inside = not inside
        continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad += 1
print("行数 =", len(lines), " 围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0, " 块内标题 =", bad)
