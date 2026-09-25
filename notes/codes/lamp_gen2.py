import random
def make(H,W,dens,path):
    random.seed(11)
    lines=[f"{H} {W}"]
    for i in range(H):
        lines.append(''.join('#' if random.random()<dens else '.' for _ in range(W)))
    open(path,'w',encoding='ascii').write('\n'.join(lines)+'\n')
make(200,200,0.0,r'D:\lenovo\chat_with_deepseek_harness\test_\lamp_200.txt')
make(400,400,0.0,r'D:\lenovo\chat_with_deepseek_harness\test_\lamp_400.txt')
print("ok")
