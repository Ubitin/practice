import subprocess, os
T = r"D:\lenovo\chat_with_deepseek_harness\test_"
EXE = os.path.join(T, "p01_calc2.exe")
cases = ["9.@", "12.@", "5.3.+@", "12.34.+@", "6.2./@", "5.3.-@", "9.@\n"]
for c in cases:
    r = subprocess.run([EXE], input=c, capture_output=True, text=True)
    print("?? %-12s -> stdout=[%s]  ???=0x%08X" % (repr(c), r.stdout.strip(), r.returncode & 0xFFFFFFFF))
