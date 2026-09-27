# -*- coding: utf-8 -*-
# 在手册 §9 报错速查 里追加「C++ 标准版本不够」小节
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("## 10. lambda"))
print("anchor L%d: %s" % (i + 1, lines[i]))

blk = [
 '### 9.3 报错 `was not declared in this scope`：C++ 标准版本不够（实测三种标准）',
 '```text',
 '【症状】本地能编译（用 -std=c++17），交到 OJ 报错：',
 '    error: \'gcd\' was not declared in this scope        ← 用了 std::gcd',
 '    error: \'lcm\' was not declared in this scope',
 '    error: \'to_string\' was not declared ...（极老的编译器）',
 '',
 '【实测：同一份代码换个 -std 就编不过】',
 '    g++ -std=c++17 → 退出码 0、零警告   ✅',
 '    g++ -std=c++14 → error: \'gcd\' was not declared in this scope  ✘',
 '    g++ -std=c++11 → 同上 ✘',
 '    （std::gcd / std::lcm 是 C++17 才加进 <numeric> 的；C++98 连 using ll = long long 都不支持）',
 '',
 '【哪些常用东西需要哪个标准（背这张表，交题前对照）】',
 '    C++11：auto、范围 for、lambda、nullptr、long long、to_string、unordered_*、using 别名',
 '    C++14：泛型 lambda、变量模板、数字分隔符（1\'000\'000）',
 '    C++17：★ std::gcd / std::lcm、结构化绑定 auto [a,b]=p、if constexpr、',
 '            std::optional、string_view、clamp、并行算法',
 '    C++20：★ 三路比较 <=>、concept、ranges、std::format、bit 库（popcount）',
 '',
 '【对策（按优先级）】',
 '    1. 交题前先确认 OJ 的编译选项；不确定就【只用 C++11 的特性】+ 自己写工具函数',
 '       · gcd → 手写：ll g(ll a,ll b){ if(a<0)a=-a; if(b<0)b=-b; while(b){ll t=a%b;a=b;b=t;} return a; }',
 '       · popcount → __builtin_popcount / __builtin_popcountll（GCC 扩展，任何标准可用）',
 '       · clamp → 手写 min(max(x,lo),hi)',
 '    2. 若 OJ 支持选语言版本，选 C++17 / C++20 再交',
 '    3. 本地自测时【多加一个低标准编译】：g++ -std=c++11 -O2 -Wall a.cpp -o a',
 '       —— 这一步能提前抓住"本地过、线上挂"的版本类错误',
 '```',
 '// ⚠️ 同理的还有"拓展标准"：-std=gnu++17 才有 M_PI、__int128 也是 GCC/Clang 扩展（不是标准 C++）',
 '//    换编译器（MSVC）时 __int128 直接不存在 ⇒ 大数乘法要么避开，要么写"龟速乘"',
 ''
]
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("inserted, total lines =", len(lines))

# 体检
fences = [j for j, l in enumerate(lines) if l.lstrip().startswith("```")]
inside = False
bad = 0
for l in lines:
    if l.lstrip().startswith("```"):
        inside = not inside
        continue
    if inside and (l.startswith("## ") or l.startswith("### ")):
        bad += 1
print("围栏 =", len(fences), "偶数 =", len(fences) % 2 == 0, " 块内标题 =", bad)
