import random
random.seed(3)
n=1000000; q=200000
out=[str(n), ' '.join(str(random.randint(0,10**9)) for _ in range(n)), str(q)]
for _ in range(q):
    if random.random()<0.5:
        out.append(f"2 {random.randint(0,10**9)}")
    else:
        out.append(f"1 {random.randint(1,n)} {random.randint(0,10**9)}")
open(r'D:\lenovo\chat_with_deepseek_harness\test_\p7990_big.txt','w').write('\n'.join(out)+'\n')
print('ok')
