# CF377A 的答案校验器（这题答案不唯一，必须用 checker，而不是比对固定输出）
# 用法: python cf377a_check.py <in.txt> <out.txt>
# 检查四件事：
#   ① 尺寸一样、'#' 一个都不能动
#   ② 恰好把 k 个 '.' 换成了 'X'，没有别的改动
#   ③ 剩下的 '.' 是连通的（4 方向）—— 题目要求
#   ④ 剩下的 '.' 数量 = 原空格数 - k
import sys
from collections import deque

inp = open(sys.argv[1], encoding='ascii').read().split()
out = open(sys.argv[2], encoding='ascii').read().split()
n, m, k = int(inp[0]), int(inp[1]), int(inp[2])
A = inp[3:3 + n]
B = out[:n]

if len(B) != n:
    print("X 输出的行数不是 %d（是 %d）" % (n, len(B)))
    sys.exit(1)

fail = []
E0 = sum(r.count('.') for r in A)
X = 0
for i in range(n):
    if len(B[i]) != m:
        fail.append("第 %d 行长度不是 %d" % (i + 1, m))
        continue
    for j in range(m):
        a, b = A[i][j], B[i][j]
        if a == '#':
            if b != '#':
                fail.append("(%d,%d) 原来是墙却变成了 %r" % (i + 1, j + 1, b))
        elif a == '.':
            if b == 'X':
                X += 1
            elif b != '.':
                fail.append("(%d,%d) 出现非法字符 %r" % (i + 1, j + 1, b))
if X != k:
    fail.append("变成墙的格子数 = %d，题目要求恰好 %d" % (X, k))

cells = [(i, j) for i in range(n) for j in range(m) if B[i][j] == '.']
if cells:
    seen = {cells[0]}
    q = deque([cells[0]])
    while q:
        x, y = q.popleft()
        for a, b in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
            if 0 <= a < n and 0 <= b < m and B[a][b] == '.' and (a, b) not in seen:
                seen.add((a, b))
                q.append((a, b))
    if len(seen) != len(cells):
        fail.append("剩下的空格不连通：只有 %d/%d 个格子能互相走到" % (len(seen), len(cells)))
    if len(cells) != E0 - k:
        fail.append("剩下的空格数 %d != 原空格数 %d - k %d" % (len(cells), E0, k))
elif k != E0:
    fail.append("一个空格都不剩，但 k=%d != 空格总数 %d" % (k, E0))

if fail:
    print("FAIL:")
    for f in fail[:6]:
        print("   - " + f)
    sys.exit(1)
print("OK: exactly %d walls added, walls intact, %d empty cells connected" % (k, len(cells)))
