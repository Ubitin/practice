import random
random.seed(21)
out=[]
for t in range(4000):
    n=random.randint(2,7)
    xs=sorted(random.sample(range(0,45), n))
    K=random.choice([0,0,1,1,2,3,5,10,100])
    out.append(f"{n} {K}\n"+" ".join(map(str,xs))+"\n")
open(r'D:\lenovo\chat_with_deepseek_harness\test_\nwpu_D_batch_in.txt','w').write(''.join(out))
print("?? 4000 ?")
