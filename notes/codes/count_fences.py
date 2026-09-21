import re, sys

p = sys.argv[1]
L = open(p, encoding='utf-8').read().split('\n')
FENCE = re.compile(r'^\s*(`{3,}|~{3,})')
fences = [(i, l.strip()) for i, l in enumerate(L, 1) if FENCE.match(l)]
in_f = False
bad = []
for i, l in fences:
    in_f = not in_f
    if l.startswith('~'):
        bad.append(('tilde fence', i, l))
code = 0
in_f = False
for l in L:
    if FENCE.match(l):
        in_f = not in_f
        continue
    if in_f and l.strip():
        code += 1
print(f'文件: {p}')
print(f'总行数     = {len(L)}')
print(f'围栏行数   = {len(fences)}  ({"偶数 OK" if len(fences) % 2 == 0 else "奇数 异常!"})')
print(f'末尾配对   = {"闭合 OK" if not in_f else "仍在围栏内 异常!"}')
print(f'代码行数   = {code}')
print(f'代码块个数 = {len(fences) // 2}')
print('前 6 个围栏:', fences[:6])
print('后 4 个围栏:', fences[-4:])
if len(fences) % 2:
    print('⚠️ 围栏行号为奇数个，请检查是否有 `` 或漏掉的收尾围栏')
