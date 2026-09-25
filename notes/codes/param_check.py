# 独立验证：用 Python 自己算一遍后序名单（不用 C++，避免"自己验证自己"）
# 用法: python param_check.py <in.txt>
import sys
from collections import deque

def main(path):
    with open(path) as f:
        tok = f.read().split()
    n, m, k = int(tok[0]), int(tok[1]), int(tok[2])
    g = tok[3:3 + n]
    blank = sum(r.count('.') for r in g)
    s = next(((i, j) for i in range(n) for j in range(m) if g[i][j] == '.'), None)
    # 迭代版 DFS，模拟"四个方向都走完才记账"的后序
    sys.setrecursionlimit(10000)
    order = []
    vis = [[False] * m for _ in range(n)]
    st = [(s[0], s[1], 0)]
    vis[s[0]][s[1]] = True
    dirs = [(0, 1), (0, -1), (1, 0), (-1, 0)]
    while st:
        x, y, d = st[-1]
        if d == 4:
            order.append((x, y))
            st.pop()
            continue
        st[-1] = (x, y, d + 1)
        nx, ny = x + dirs[d][0], y + dirs[d][1]
        if 0 <= nx < n and 0 <= ny < m and g[nx][ny] == '.' and not vis[nx][ny]:
            vis[nx][ny] = True
            st.append((nx, ny, 0))
    print(f"空格总数 blank = {blank}")
    print(f"后序名单长度 = {len(order)}  (应等于 blank)")
    pos = order.index(s) + 1 if s in order else -1
    print(f"起点 {s} 在名单里的位置 = {pos}")
    show = min(12, len(order))
    print("名单前 %d 项: " % show + " ".join(f"({x},{y})" for x, y in order[:show]))
    print(f"名单最后一项: {order[-1]}")

main(sys.argv[1])
