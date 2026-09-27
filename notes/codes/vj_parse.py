import re
p = r"D:\lenovo\chat_with_deepseek_harness\test_\vj2.html"
h = open(p, encoding="utf-8", errors="ignore").read()
print("?? =", len(h))
# ??
m = re.search(r"<title>(.*?)</title>", h, re.S)
print("title =", m.group(1).strip() if m else "(?)")
# ???????
for kw in ["??", "63", "problem", "OJ", "vjudge"]:
    print("  ? %-8s : %d ?" % (kw, h.count(kw)))
# ???"??"???
i = h.find("??")
if i >= 0:
    print("\n--- ???????? ---")
    print(h[max(0,i-300):i+800])
else:
    print("\n--- ? 1200 ?? ---")
    print(h[:1200])
