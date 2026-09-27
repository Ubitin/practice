import random, subprocess, sys
random.seed(20260927)
EXE_USER  = r"D:\lenovo\chat_with_deepseek_harness\test_\p013_user.exe"
EXE_FIXED = r"D:\lenovo\chat_with_deepseek_harness\test_\p013_fixed.exe"
def run(exe, a,b,m):
    r = subprocess.run([exe], input=f"{a} {b} {m}", capture_output=True, text=True)
    return r.stdout.strip()
bad_u = bad_f = 0
first = None
cases = []
# ?????? / ? / ?? 2^63
for _ in range(60):
    cases.append((random.randint(1,1000), random.randint(1,1000), random.randint(1,1000)))
for _ in range(60):
    cases.append((random.randint(1,10**9), random.randint(1,10**9), random.randint(1,10**9)))
for _ in range(60):
    cases.append((random.randint(1,2**63), random.randint(1,2**63), random.randint(1,2**63)))
for (a,b,m) in cases:
    truth = pow(a,b,m)
    u = run(EXE_USER,a,b,m)
    f = run(EXE_FIXED,a,b,m)
    if u != str(truth):
        bad_u += 1
        if first is None: first = (a,b,m,truth,u)
    if f != str(truth): bad_f += 1
print(f"???={len(cases)}  vs Python pow(a,b,m)")
print(f"  ???? ? {bad_u} / {len(cases)}")
print(f"  ???   ? {bad_f} / {len(cases)}")
if first: print(f"  ??????: a={first[0]} b={first[1]} m={first[2]}  ??={first[3]}  ??={first[4]}")
