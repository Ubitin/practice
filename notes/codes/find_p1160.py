import io, glob, os
# ????????? OI-wiki / ??? P1160 ????????????????
d = r"D:\lenovo\chat_with_deepseek_harness"
pats = ["P1160", "????", "ins_back", "indexx"]
for root, dirs, files in os.walk(d):
    if any(x in root for x in ["\\test_", "\\??"]):   # ??????????
        continue
    for f in files:
        if f.endswith((".md", ".txt", ".cpp", ".html")):
            fp = os.path.join(root, f)
            try:
                t = io.open(fp, encoding="utf-8", errors="ignore").read()
            except Exception:
                continue
            for p in pats:
                if p in t:
                    print("?? %-12s ? %s" % (p, fp.replace(d, ".")))
                    break
