# -*- coding: utf-8 -*-
# 干净对照：原版 01.cpp vs 修正版，同一批输入
import subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
ORIG = os.path.join(T, "p01_calc.exe")      # 由用户 01.cpp 编译
FIXED = os.path.join(T, "calc_fixed.exe")

cases = ["9.@", "12.@", "5.3.+@", "12.34.+@", "6.2./@", "5.3.-@"]

print("%-12s | %-26s | %-26s" % ("输入", "原版 01.cpp", "修正版"))
print("-" * 72)
for c in cases:
    r1 = subprocess.run([ORIG], input=c, capture_output=True, text=True)
    r2 = subprocess.run([FIXED], input=c, capture_output=True, text=True)
    o1 = r1.stdout.strip() or "(空)"
    o2 = r2.stdout.strip() or "(空)"
    print("%-12s | 输出=%-8s 退出码=0x%08X | 输出=%-8s 退出码=%d"
          % (c, o1, r1.returncode & 0xFFFFFFFF, o2, r2.returncode))
