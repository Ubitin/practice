import random
random.seed(9)
n=100000
w=''.join(random.choice('UDLR') for _ in range(n))
open(r'D:\lenovo\chat_with_deepseek_harness\test_\ms_big.txt','w').write(f"-1000000000 -1000000000\n1000000000 1000000000\n{n}\n{w}\n")
open(r'D:\lenovo\chat_with_deepseek_harness\test_\ms_big2.txt','w').write(f"0 0\n1000000000 -1000000000\n{n}\n{w}\n")
print("ok")
