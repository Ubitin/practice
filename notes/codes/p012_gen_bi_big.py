import random
random.seed(3)
T=100000
lines=[str(T)]
for _ in range(T): lines.append(str(random.randint(10**18, 10**30)))
open(r'D:\lenovo\chat_with_deepseek_harness\test_\p012_bi_big.txt','w').write("\n".join(lines)+"\n")
print("?? T=1e5?n ? [1e18, 1e30]")
