import subprocess, os
T = r"D:\lenovo\chat_with_deepseek_harness\test_"
A = os.path.join(T,"p1160_list.exe")
sample = "4\n1 0\n2 1\n1 0\n2\n3\n"
r = subprocess.run([A], input=sample, capture_output=True, text=True)
print("????? =", r.stdout.strip(), " ?? 2 4 1")
