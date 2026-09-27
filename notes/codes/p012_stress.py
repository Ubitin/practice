import random, subprocess
random.seed(7)
EXE = r"D:\lenovo\chat_with_deepseek_harness\test_\p012_fixed.exe"
cases = [random.randint(1,3000) for _ in range(400)]
inp = f"{len(cases)}\n" + "\n".join(map(str,cases)) + "\n"
r = subprocess.run([EXE], input=inp, capture_output=True, text=True)
out = r.stdout.split()
def brute(n):
    return sum(i for i in range(3,n) if i%3==0 or i%5==0)
bad = 0
for n, o in zip(cases, out):
    if int(o) != brute(n): bad += 1
print(f"012 ??? vs ?????{len(cases)} ????? {bad}")
