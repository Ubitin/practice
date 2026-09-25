import random
random.seed(7)
H=W=2000
lines=[f"{H} {W}"]
for i in range(H):
    lines.append(''.join('#' if random.random()<0.05 else '.' for _ in range(W)))
open(r'D:\lenovo\chat_with_deepseek_harness\test_\lamp_big.txt','w',encoding='ascii').write('\n'.join(lines)+'\n')
print("????")
