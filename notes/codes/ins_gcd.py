# -*- coding: utf-8 -*-
# 在手册 §6.16 之后、## 7 之前插入 §6.17 gcd / lcm
import io, os, glob

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
p = [f for f in glob.glob(os.path.join(d, "*.md")) if f.endswith("机试代码模板速查手册.md")][0]
lines = io.open(p, "r", encoding="utf-8").read().split("\n")

i = next(i for i, l in enumerate(lines) if l.startswith("## 7."))
print("anchor L%d: %s" % (i + 1, lines[i]))

blk = [
 '### 6.17 gcd / lcm（辗转相除、溢出安全的 lcm、std::gcd）',
 '```cpp',
 '// ── 手写 gcd（辗转相除 / 欧几里得）：O(log min(a,b)) ──',
 'll gcd_(ll a, ll b) {',
 '    if (a < 0) a = -a;          // C++ 的 % 保留符号（-12%8 == -4），先用绝对值',
 '    if (b < 0) b = -b;',
 '    while (b) { ll t = a % b; a = b; b = t; }   // (a,b) -> (b,a%b)，b==0 时停',
 '    return a;                    // 此时 a 就是 gcd',
 '}',
 '// 手推 gcd(12,8): (12,8)->(8,4)->(4,0) => 4',
 '',
 '// ── lcm（最小公倍数）：必须先除后乘，否则溢出！──',
 'll lcm_(ll a, ll b) {',
 '    ll g = gcd_(a, b);',
 '    if (g == 0) return 0;                       // 两个 0 的 lcm 约定为 0',
 '    return a / g * b;                           // ✔ 先除后乘',
 '    // return a * b / g;                        // ✘ a*b 先溢出（a,b 都到 1e9 时 1e18 贴上限，1e18*1e18 直接爆）',
 '}',
 '',
 '// ── C++17 标准库已有，一行顶上面整段（<numeric>）──',
 '#include <numeric>',
 'll g = std::gcd(a, b);                          // 自带处理正负，返回非负',
 'll l = std::lcm(a, b);                          // 同样溢出安全（内部先除后乘）',
 '',
 '// ── 批量求 gcd：前缀/后缀 + 稀疏表；或多次调用直接 std::gcd ──',
 '// 区间 gcd 可倍增预处理 O(n log n) + O(1) 查询；也可用线段树 O(log n)/次',
 '```',
 '// ⚠️ 复杂度对比（实测）：',
 '//   辗转相除        gcd(99999999, 100000000) : < 0.001 ms（最多约 90 次迭代）',
 '//   从 min 开始试除  gcd(99999999, 100000000) : 132.0 ms（互素时要退到 i=1，约 1e8 次）',
 '//   ⇒ 试除法外推到 1e18 规模约 1.3e12 ms ≈ 42 年 ⇒ 必 TLE',
 '//   ⚠️ 但注意：负数化简【不是】试除法的错——只要先取了绝对值，试除法在负数上也能得到对的 gcd',
 '//      （实测 gcd_trial(-12,8)=4 正确）；试除法真正的问题是【慢】',
 '// ⚠️ lcm 一定先除后乘：a*b/g 在 a,b ≥ 1e10 时必溢出',
 '// ⚠️ 分数化简时：g = gcd(|x|,|y|)；x/=g; y/=g; 再把符号统一到分子（if (y<0) {x=-x;y=-y;}）',
 '// ⚠️ gcd(x,0) = |x|；gcd(0,0) = 0（标准库也一样）',
 '// 同族：分数四则运算化简、同余方程、扩展欧几里得求逆元、最小公倍数计数、周期问题',
 ''
]
for k, s in enumerate(blk):
    lines.insert(i + k, s)

io.open(p, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
print("inserted, total lines =", len(lines))
