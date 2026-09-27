import json, glob, os, re
d = r"D:\lenovo\chat_with_deepseek_harness\??"
notes = "\n".join(open(f, encoding="utf-8", errors="ignore").read() for f in glob.glob(os.path.join(d, "*.md")))
# ???????
for extra in ["??????????.md"]:
    fp = os.path.join(d, extra)
    if os.path.exists(fp):
        notes += open(fp, encoding="utf-8", errors="ignore").read()

data = json.load(open(r"D:\lenovo\chat_with_deepseek_harness\test_\vj_problems.json", encoding="utf-8"))
solved_hist, unknown = [], []
for key, v in data.items():
    title = v[0]
    # ????????????????/???
    hit = False
    for tag in [title, key, key.replace("CodeForces-","CF"), key.replace("Gym-","")]:
        if tag and len(tag) > 3 and tag in notes:
            hit = True; break
    (solved_hist if hit else unknown).append((key, title))

print("?????????????????%d ?" % len(solved_hist))
for k, t in solved_hist:
    print("   ? %-40s %s" % (t[:40], k))
print("\n?????????%d ?" % len(unknown))
for k, t in unknown:
    print("   ? %-40s %s" % (t[:40], k))
