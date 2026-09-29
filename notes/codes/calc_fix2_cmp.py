import subprocess, os
T = r"D:\lenovo\chat_with_deepseek_harness\test_"
OLD = os.path.join(T, "p01_calc2.exe")
NEW = os.path.join(T, "calc_fixed2.exe")
cases = ["9.@", "12.@", "5.3.+@", "12.34.+@", "5.3.-@", "6.2./@", "9.@\n", "1.2.3.++@", "1.2.+3.*@"]
exp   = ["9",   "12",   "8",        "46",        "2",       "3",       "9",      "6",          "9"]
print("%-14s | %-8s | %-8s | %-6s | %s" % ("??", "??v2", "??v2", "??", "??"))
print("-" * 62)
for c, e in zip(cases, exp):
    r1 = subprocess.run([OLD], input=c, capture_output=True, text=True, timeout=10)
    r2 = subprocess.run([NEW], input=c, capture_output=True, text=True, timeout=10)
    o1 = r1.stdout.strip() or "(?)"
    o2 = r2.stdout.strip() or "(?)"
    mark = "OK" if o2 == e else "??"
    print("%-14s | %-8s | %-8s | %-6s | %s" % (repr(c), o1, o2, e, mark))
