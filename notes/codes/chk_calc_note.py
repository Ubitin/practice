import io
p = r"D:\lenovo\chat_with_deepseek_harness\??\???????-??????????.md"
lines = io.open(p, encoding="utf-8").read().split("\n")
f = [i for i, l in enumerate(lines) if l.lstrip().startswith("```")]
print("?? =", len(lines), " ?? =", len(f), " ?? =", len(f) % 2 == 0)
