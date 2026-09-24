# 用 Python 标准库 colorsys（独立实现）交叉验证 RGB→HSV
# 用法: python hsv_check.py <你的exe> [随机组数]
import colorsys, random, subprocess, sys

exe = sys.argv[1]
N = int(sys.argv[2]) if len(sys.argv) > 2 else 3000

edge = [(0, 215, 0), (0, 0, 0), (255, 255, 255), (100, 100, 100), (128, 128, 128),
        (255, 0, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0), (0, 255, 255), (255, 0, 255),
        (100, 150, 200), (200, 150, 100), (0, 215, 100), (215, 0, 100), (100, 0, 215),
        (1, 1, 2), (2, 1, 1), (1, 2, 1), (254, 255, 255), (0, 0, 1), (1, 0, 0)]
rand = [(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)) for _ in range(N)]
cases = edge + rand

bad = 0
shown = 0
for (r, g, b) in cases:
    h, s, v = colorsys.rgb_to_hsv(r / 255.0, g / 255.0, b / 255.0)
    want = "%.4f,%.4f%%,%.4f%%" % (h * 360.0, s * 100.0, v * 100.0)
    got = subprocess.run([exe], input="%d %d %d\n" % (r, g, b),
                         capture_output=True, text=True).stdout.strip()
    if got != want:
        bad += 1
        if shown < 8:
            shown += 1
            print("  ✘ 输入 %3d %3d %3d  期望 [%s]  实得 [%s]" % (r, g, b, want, got))
print("共 %d 组（边界 %d + 随机 %d）：不一致 %d 组" % (len(cases), len(edge), len(rand), bad))
