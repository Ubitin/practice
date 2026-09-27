import random, sys
random.seed(int(sys.argv[1]) if len(sys.argv)>1 else 11)
K=500
out=[]
MOVE={'U':(0,1),'D':(0,-1),'L':(-1,0),'R':(1,0)}
for t in range(K):
    n=random.randint(1,4)
    x1=random.randint(0,4); y1=random.randint(0,4)
    w=''.join(random.choice('UDLR') for _ in range(n))
    # ???????"?+?"??? d ???????? ? ?? ? d ???
    d=random.randint(1,10)
    x,y=x1,y1
    for day in range(1,d+1):
        wx,wy=MOVE[w[(day-1)%n]]
        # ????? U/D/L/R ??????????????
        sx,sy=MOVE[random.choice('UDLR')] if random.random()<0.85 else (0,0)
        x+=wx+sx; y+=wy+sy
    out.append(f"{x1} {y1} {x} {y} {n}\n{w}\n")
open(r'D:\lenovo\chat_with_deepseek_harness\test_\ms_batch_in.txt','w').write(''.join(out))
print("??",K,"???? 10 ?????")
