# -*- coding: utf-8 -*-
# 复现"没有输出"：分别测单数字/多数字/加法/缺 @ 的情况
import subprocess, os

T = r"D:\lenovo\chat_with_deepseek_harness\test_"
EXE = os.path.join(T, "p01_calc.exe")

cases = [
    ("9.@", "单数字 9"),
    ("12.@", "多位数 12"),
    ("5.3.+@", "5+3"),
    ("12.34.+@", "12+34"),
    ("5.3.+", "末尾没有 @"),
    ("123.@", "三位数 123"),
]
for inp, desc in cases:
    r = subprocess.run([EXE], input=inp, capture_output=True, text=True)
    out = r.stdout.strip()
    err = r.stderr.strip()
    print("输入 %-12s (%s)" % (inp, desc))
    print("    stdout = [%s]   stderr = [%s]   退出码 = %d" % (out, err[:60], r.returncode))
