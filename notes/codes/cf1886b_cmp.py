# 比较两个程序对同一个多组输入文件的输出（浮点，容差 1e-6）
# 用法: python cf1886b_cmp.py <in.txt> <sol_out.txt> <ref_out.txt> [tol]
import sys

inp = open(sys.argv[1], encoding='ascii').read().split('\n')
sol = open(sys.argv[2], encoding='ascii').read().split()
ref = open(sys.argv[3], encoding='ascii').read().split()
tol = float(sys.argv[4]) if len(sys.argv) > 4 else 1e-6

cases = inp[2:]           # 第 1 行是 T，之后每行一组
bad = 0
shown = 0
worst = 0.0
worst_case = ''
for i, (a, b) in enumerate(zip(sol, ref)):
    diff = abs(float(a) - float(b))
    if diff > worst:
        worst, worst_case = diff, cases[i] if i < len(cases) else '?'
    if diff > tol:
        bad += 1
        if shown < 6:
            shown += 1
            print("第 %d 组 ✘  输入: %-28s 你的=%-16s 参考=%-16s 偏差=%.4f"
                  % (i + 1, cases[i] if i < len(cases) else '?', a, b, diff))
print("—— 共 %d 组：不一致 %d 组；最大偏差 %.4f（出现在 %s）"
      % (len(sol), bad, worst, worst_case))
