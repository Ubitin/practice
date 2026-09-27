import re, ssl, urllib.request
p = r"D:\lenovo\chat_with_deepseek_harness\test_\vj2.html"
h = open(p, encoding="utf-8", errors="ignore").read()
# ? article / data ??? script ? ajax ??
for pat in [r'https?://[^"\']*article[^"\']*', r'/article/[^"\']*', r'data-[a-z-]+="[^"]{0,60}"']:
    ms = re.findall(pat, h)
    print(pat, "->", ms[:8])

# ???? articleId / contestId ??
for kw in ["articleId", "article_id", "11277", "fetch(", "$.ajax", "XMLHttpRequest", "problemset"]:
    print("  %-16s : %d" % (kw, h.count(kw)))

# ?? body ??????????????????
body = re.sub(r"<(script|style).*?</\1>", " ", h, flags=re.S)
text = re.sub(r"<[^>]+>", " ", body)
text = re.sub(r"\s+", " ", text)
print("\n?????? =", len(text))
print(text[:1500])
