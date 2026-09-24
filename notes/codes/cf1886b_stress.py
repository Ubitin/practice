# CF1886B 对拍驱动（浮点答案，按 1e-6 容差比较）
# 用法: python cf1886b_stress.py <sol.exe> <ref.exe> <gen.exe> <from> <to> [tol]
import subprocess, sys

sol, ref, gen = sys.argv[1], sys.argv[2], sys.argv[3]
lo, hi = int(sys.argv[4]), int(sys.argv[5])
tol = float(sys.argv[6]) if len(sys.argv) > 6 else 1e-6

bad = 0
shown = 0
worst = 0.0
for seed in range(lo, hi + 1):
    inp = subprocess.run([gen, str(seed)], capture_output=True, text=True).stdout
    a = subprocess.run([sol], input=inp, capture_output=True, text=True).stdout.split()
    b = subprocess.run([ref], input=inp, capture_output=True, text=True).stdout.split()
    if len(a) != len(b):
        print("[seed %d] 行数不同 sol=%d ref=%d" % (seed, len(a), len(b)))
        bad += 1
        continue
    worstHere = 0.0
    for x, y in zip(a, b):
        diff = abs(float(x) - float(y))
        worstHere = max(worstHere, diff)
        worst = max(worst, diff)
    if worstHere > tol:
        bad += 1
        if shown < 5:
            shown += 1
            print("[seed %d] ✘ 最大偏差 %.6f > %.1e" % (seed, worstHere, tol))
            print("    输入: " + inp.replace("\n", " / ").strip())
            print("    你的 : " + " ".join(a))
            print("    参考 : " + " ".join(b))
print("—— seeds %d..%d：不一致 %d / %d 轮；全轮最大偏差 %.2e（容差 %.1e）"
      % (lo, hi, bad, hi - lo + 1, worst, tol))
