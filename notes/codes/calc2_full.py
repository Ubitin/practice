# -*- coding: utf-8 -*-
# 全面测试用户新版本（getchar + do/while + temp 在循环内）
import subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
EXE = os.path.join(T, "p01_calc2.exe")

cases = [
    "9.@",          # 单数字
    "12.@",         # 多位数
    "5.3.+@",       # 加法
    "12.34.+@",     # 多位加法
    "5.3.-@",       # 减法
    "6.2./@",       # 除法（用户版崩）
    "9.@\n",        # 带换行
    "\n",           # 只有换行
    "",             # 空输入
    "1.2.3.++@",    # 三个数相加
]
print("%-16s | %-10s | %-12s" % ("输入", "stdout", "退出码"))
print("-" * 48)
for c in cases:
    r = subprocess.run([EXE], input=c, capture_output=True, text=True, timeout=10)
    o = r.stdout.strip() or "(空)"
    print("%-16s | %-10s | 0x%08X" % (repr(c), o, r.returncode & 0xFFFFFFFF))
