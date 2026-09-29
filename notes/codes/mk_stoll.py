import subprocess, os
T = r"D:\lenovo\chat_with_deepseek_harness\test_"
# ??????????? stoll("")
src = '#include<bits/stdc++.h>\nint main(){ try { auto v = std::stoll(std::string("")); printf("%lld\\n", v);} catch(...){ printf("caught\\n");} }\n'
open(os.path.join(T,"zz_stoll.cpp"),"w",encoding="utf-8").write(src)
