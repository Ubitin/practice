import random
random.seed(5)
for scale in [10000, 20000, 40000]:
    n=q=scale
    out=[str(n), ' '.join(str(random.randint(0,1000)) for _ in range(n)), str(q)]
    for _ in range(q):
        if random.random()<0.5: out.append(f"2 {random.randint(0,1000)}")
        else: out.append(f"1 {random.randint(1,n)} {random.randint(0,1000)}")
    open(rf'D:\lenovo\chat_with_deepseek_harness\test_\p7990_sc{scale}.txt','w').write('\n'.join(out)+'\n')
print('ok')
