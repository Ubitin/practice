# CF377A 批量验证：对每组随机迷宫跑待测程序，再用 checker 判定
# 用法: python cf377a_stress.py <待测exe> <gen.exe> <from> <to>
import subprocess, sys, os

# ⚠️ 修 bug 2：临时文件名必须【按进程唯一】，否则两个对拍同时跑会互相覆盖
#    （曾导致"输入是 A 组、输出是 B 组"的假 FAIL）
TAG = "%d_%d" % (os.getpid(), int(sys.argv[3]))
FIN, FOUT = '_t_%s_in.txt' % TAG, '_t_%s_out.txt' % TAG

sol, gen = sys.argv[1], sys.argv[2]
lo, hi = int(sys.argv[3]), int(sys.argv[4])
bad = 0
shown = 0
for seed in range(lo, hi + 1):
    inp = subprocess.run([gen, str(seed)], capture_output=True, text=True).stdout
    open(FIN, 'w', newline='\n').write(inp)
    out = subprocess.run([sol], input=inp, capture_output=True, text=True).stdout
    open(FOUT, 'w', newline='\n').write(out)
    # ⚠️ 修 bug：checker 会打印中文，这里必须显式 utf-8，否则 Windows 默认 GBK 解码直接抛异常
    r = subprocess.run([sys.executable, 'cf377a_check.py', FIN, FOUT],
                       capture_output=True, text=True, encoding='utf-8', errors='replace')
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
