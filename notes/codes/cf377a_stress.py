# CF377A 批量验证：对每组随机迷宫跑待测程序，再用 checker 判定
# 用法: python cf377a_stress.py <待测exe> <gen.exe> <from> <to>
import subprocess, sys

sol, gen = sys.argv[1], sys.argv[2]
lo, hi = int(sys.argv[3]), int(sys.argv[4])
bad = 0
shown = 0
for seed in range(lo, hi + 1):
    inp = subprocess.run([gen, str(seed)], capture_output=True, text=True).stdout
    open('_t_in.txt', 'w', newline='\n').write(inp)
    out = subprocess.run([sol], input=inp, capture_output=True, text=True).stdout
    open('_t_out.txt', 'w', newline='\n').write(out)
    r = subprocess.run([sys.executable, 'cf377a_check.py', '_t_in.txt', '_t_out.txt'],
                       capture_output=True, text=True)
    if r.returncode != 0:
        bad += 1
        if shown < 3:
            shown += 1
            print("--- seed %d 不合格 ---" % seed)
            print(inp.rstrip())
            print("输出:")
            print(out.rstrip())
            print(r.stdout.rstrip())
print("seeds %d..%d：不合格 %d / %d 组" % (lo, hi, bad, hi - lo + 1))
