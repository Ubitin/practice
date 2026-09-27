import random
random.seed(int(__import__('sys').argv[1]) if len(__import__('sys').argv)>1 else 3)
K=400
cases=[]
for t in range(K):
    style=t%4
    n=random.randint(1,4)
    x1=random.randint(0,3); y1=random.randint(0,3)
    # ????????? ? 8???????
    while True:
        x2=x1+random.randint(-4,4); y2=y1+random.randint(-4,4)
        if abs(x2-x1)+abs(y2-y1)<=8 and x2>=0 and y2>=0: break
    if style==0: w=''.join(random.choice('UDLR') for _ in range(n))
    elif style==1: w=random.choice('UDLR')*n
    elif style==2: w=''.join(random.choice('UD') for _ in range(n))
    else: w=('UD'*n)[:n]
    cases.append(f"{x1} {y1} {x2} {y2} {n}\n{w}\n")
open(r'D:\lenovo\chat_with_deepseek_harness\test_\ms_batch_in.txt','w').write(''.join(cases))
print("??", K, "?")
