import sys

# 造 CF1006E 的极限数据：n 个点的【链】（递归 DFS 最深的情形）+ q 个查询
# 用法: python make_1006e_big.py <n> <q> <chain|star|random> <out.txt> <out.ans>
n = int(sys.argv[1]); q = int(sys.argv[2]); kind = sys.argv[3]
out, ans = sys.argv[4], sys.argv[5]

# 树
par = [0] * (n + 1)
if kind == 'chain':
    for i in range(2, n + 1): par[i] = i - 1
elif kind == 'star':
    for i in range(2, n + 1): par[i] = 1
else:
    st = 12345
    for i in range(2, n + 1):
        st = (st * 1103515245 + 12345) & 0x7fffffff
        par[i] = 1 + st % (i - 1)

# 先根序 + 子树大小（迭代）
ch = [[] for _ in range(n + 1)]
for i in range(2, n + 1): ch[par[i]].append(i)
tin = [0] * (n + 1); ordv = [0] * (n + 1); sz = [1] * (n + 1)
stack = [1]; timer = 0
while stack:
    u = stack.pop()
    timer += 1; tin[u] = timer; ordv[timer] = u
    for v in reversed(ch[u]): stack.append(v)
for i in range(timer, 1, -1):
    u = ordv[i]; sz[par[u]] += sz[u]

# 查询：故意三种都有（合法 / 刚好越界 / 远越界）
queries = []
for i in range(q):
    if i % 3 == 0:   u, k = 1 + (i * 7) % n, 1 + (i * 13) % n
    elif i % 3 == 1: u, k = n - (i % n), n + 1
    else:            u, k = 1 + (i * 31) % n, n + 5
    queries.append((u, k))

with open(out, 'w', newline='\n') as f:
    f.write("%d %d\n" % (n, q))
    if n > 1: f.write(" ".join(str(par[i]) for i in range(2, n + 1)) + "\n")
    for u, k in queries: f.write("%d %d\n" % (u, k))

with open(ans, 'w', newline='\n') as f:
    for u, k in queries:
        f.write(("%d\n" % ordv[tin[u] + k - 1]) if k <= sz[u] else "-1\n")

print("写出 %s / %s  (kind=%s n=%d q=%d)" % (out, ans, kind, n, q))
