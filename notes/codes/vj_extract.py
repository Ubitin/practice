import re, json, html
p = r"D:\lenovo\chat_with_deepseek_harness\test_\vj2.html"
h = open(p, encoding="utf-8", errors="ignore").read()

m = re.search(r'"problemsBrief":"(.*?)","', h, re.S)
raw = m.group(1)
# ??? JSON ???
s = raw.encode().decode("unicode_escape")
s = s.encode("latin1", "ignore").decode("utf-8", "ignore")
try:
    data = json.loads(s)
except Exception as e:
    print("json ????:", e)
    print(s[:300]); raise SystemExit
print("???? =", len(data))
out = []
for key, v in data.items():
    oj = key.split("-")[0]
    title = v[0]
    out.append((oj, key, title))
from collections import Counter
c = Counter(o for o, k, t in out)
print("\n????:", dict(c))
print("\n?????")
for i, (oj, key, title) in enumerate(out, 1):
    print("%2d. [%-11s] %-46s %s" % (i, oj, title[:46], key))
json.dump(data, open(r"D:\lenovo\chat_with_deepseek_harness\test_\vj_problems.json","w",encoding="utf-8"), ensure_ascii=False, indent=1)
print("\n??? test_\\vj_problems.json")
