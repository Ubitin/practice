# 快速对拍：生成器与 checker 都内联在 Python（每轮只 spawn 一次待测程序），比 cf377a_stress.py 快很多
# 用法: python fast_stress.py <待测exe> <from> <to> [maxN] [maxM]
import random, subprocess, sys
from collections import deque

sol = sys.argv[1]
lo, hi = int(sys.argv[2]), int(sys.argv[3])
maxN = int(sys.argv[4]) if len(sys.argv) > 4 else 8
maxM = int(sys.argv[5]) if len(sys.argv) > 5 else 8

def gen(seed):
    rnd = random.Random(seed * 1000003 + 31)
    n = rnd.randint(1, maxN); m = rnd.randint(1, maxM)
    g = [['#'] * m for _ in range(n)]
    x, y = rnd.randrange(n), rnd.randrange(m)
    g[x][y] = '.'
    cells = [(x, y)]
    want = rnd.randint(1, n * m)
    guard = 0
    while len(cells) < want and guard < n * m * 20:
        guard += 1
        cx, cy = cells[rnd.randrange(len(cells))]
        d = rnd.randrange(4)
        nx, ny = cx + (1, -1, 0, 0)[d], cy + (0, 0, 1, -1)[d]
        if 0 <= nx < n and 0 <= ny < m and g[nx][ny] == '#':
            g[nx][ny] = '.'
            cells.append((nx, ny))
    E = len(cells)
    k = rnd.randint(0, max(0, E - 1))
    inp = '%d %d %d\n' % (n, m, k) + ''.join(''.join(r) + '\n' for r in g)
    return inp, n, m, k, E

def check(inp, out, n, m, k, E):
    A = inp.split('\n')[1:1 + n]
    B = [r for r in out.split('\n') if r != '']
    if len(B) != n:
        return '输出行数 %d != %d' % (len(B), n)
    X = 0
    for i in range(n):
        if len(B[i]) != m:
            return '第 %d 行长度 %d != %d' % (i + 1, len(B[i]), m)
        for j in range(m):
            a, b = A[i][j], B[i][j]
            if a == '#' and b != '#':
                return '(%d,%d) 原墙被改' % (i + 1, j + 1)
            if a == '.':
                if b == 'X':
                    X += 1
                elif b != '.':
                    return '(%d,%d) 非法字符 %r' % (i + 1, j + 1, b)
    if X != k:
        return '新墙 %d != k %d' % (X, k)
    cells = [(i, j) for i in range(n) for j in range(m) if B[i][j] == '.']
    if cells:
        seen = {cells[0]}; q = deque([cells[0]])
        while q:
            x, y = q.popleft()
            for a, b in ((x+1,y),(x-1,y),(x,y+1),(x,y-1)):
                if 0 <= a < n and 0 <= b < m and B[a][b] == '.' and (a, b) not in seen:
                    seen.add((a, b)); q.append((a, b))
        if len(seen) != len(cells):
            return '剩余空格不连通 %d/%d' % (len(seen), len(cells))
        if len(cells) != E - k:
            return '剩余空格数 %d != %d' % (len(cells), E - k)
    return None

bad = 0
shown = 0
first_bad = None
for seed in range(lo, hi + 1):
    inp, n, m, k, E = gen(seed)
    p = subprocess.run([sol], input=inp, capture_output=True, text=True, encoding='utf-8')
    out = p.stdout
    if p.returncode != 0:
        bad += 1
        if shown < 3:
            shown += 1
            print('--- seed %d RE exit=%d ---' % (seed, p.returncode)); print(inp.rstrip())
        if first_bad is None: first_bad = seed
        continue
    why = check(inp, out, n, m, k, E)
    if why:
        bad += 1
        if first_bad is None: first_bad = seed
        if shown < 3:
            shown += 1
            print('--- seed %d 不合格：%s ---' % (seed, why))
            print(inp.rstrip()); print('输出:'); print(out.rstrip())
print('seeds %d..%d（maxN=%d,maxM=%d）：不合格 %d / %d 组%s'
      % (lo, hi, maxN, maxM, bad, hi - lo + 1,
         '' if first_bad is None else '，首个不合格 seed=%d' % first_bad))
