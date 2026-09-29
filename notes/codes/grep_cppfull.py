import io, re
p = r"D:\lenovo\chat_with_deepseek_harness\cpp_full.txt"
t = io.open(p, encoding="utf-8", errors="ignore").read()
print("???? =", len(t))
for kw in ["ins_back", "ins_front", "indexx", "????", "P1160", "struct node"]:
    idxs = [m.start() for m in re.finditer(re.escape(kw), t)]
    print("  %-12s ?? %d ?  %s" % (kw, len(idxs), idxs[:5]))
