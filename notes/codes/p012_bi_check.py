import subprocess, random
EXE = r"D:\lenovo\chat_with_deepseek_harness\test_\p012_bi.exe"
def truth(n):
    return 3*((n-1)//3)*((n-1)//3+1)//2 + 5*((n-1)//5)*((n-1)//5+1)//2 - 15*((n-1)//15)*((n-1)//15+1)//2
ns = [1,2,3,4,5,6,9,10,14,15,16,29,30,31,99,100,101,999,1000,1001]
ns += [10**9, 10**9+1, 10**10, 10**10+7, 10**12, 10**15, 10**18, 10**18+1, 10**19, 10**30, 10**50, 10**100]
random.seed(1)
for _ in range(300):
    ns.append(random.randint(1, 10**random.randint(1,25)))
inp = f"{len(ns)}\n" + "\n".join(map(str,ns)) + "\n"
r = subprocess.run([EXE], input=inp, capture_output=True, text=True)
outs = r.stdout.split()
bad = 0; first=None
for n,o in zip(ns,outs):
    tv = truth(n)
    if o != str(tv):
        bad += 1
        if first is None: first=(n,o,str(tv))
print(f"???={len(ns)}?? n ? 10^100?")
print(f"  ??? = {bad}")
if first: print("  ????: n=%s ??=%s ??=%s" % first)
else: print("  ? ????")
print("  ??: n=10^100 ???? 40 ? =", outs[ns.index(10**100)][:40], "...")
