import random
random.seed(5)
T=100000
lines=[str(T)]+[str(random.randint(10**17,10**18)) for _ in range(T)]
open(r'D:\lenovo\chat_with_deepseek_harness\test_\p012_i128_big.txt','w').write("\n".join(lines)+"\n")
print("ok")
