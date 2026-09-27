import re, json, html
p = r"D:\lenovo\chat_with_deepseek_harness\test_\vj2.html"
h = open(p, encoding="utf-8", errors="ignore").read()

# ????????? JSON?vjudge ? article ?????? problems ???
for pat in [r'"problems"\s*:\s*\[', r'problemId', r'OJId', r'"title"\s*:\s*"']:
    print(pat, "->", len(re.findall(pat, h)))

# ????? <a ...>??</a> ??????
links = re.findall(r'<a[^>]*href="([^"]*)"[^>]*>(.*?)</a>', h, re.S)
print("\n???? =", len(links))
seen = set()
cnt = 0
for href, text in links:
    t = html.unescape(re.sub(r"<[^>]+>", "", text)).strip()
    if not t or len(t) > 80: continue
    if "/problem/" in href or "problemId" in href:
        key = (href, t)
        if key in seen: continue
        seen.add(key)
        cnt += 1
        print("%3d  %-58s %s" % (cnt, t[:58], href[:70]))
    if cnt > 80: break
