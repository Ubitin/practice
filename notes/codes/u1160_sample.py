import subprocess, os
T = r"D:\lenovo\chat_with_deepseek_harness\test_"
U = os.path.join(T, "p1160_user.exe")
R = os.path.join(T, "p1160_sentinel.exe")
sample = "4\n1 0\n2 1\n1 0\n2\n3\n"
ru = subprocess.run([U], input=sample, capture_output=True, text=True)
rr = subprocess.run([R], input=sample, capture_output=True, text=True)
print("???? = [%s]   ???=%d" % (ru.stdout.strip(), ru.returncode))
print("???? = [%s]   ????? 2 4 1?" % rr.stdout.strip())
