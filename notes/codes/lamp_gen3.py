import sys
H=W=2000
lines=[f"{H} {W}"]+['.'*W for _ in range(H)]
open(r'D:\lenovo\chat_with_deepseek_harness\test_\lamp_big_empty.txt','w',encoding='ascii').write('\n'.join(lines)+'\n')
print("ok")
