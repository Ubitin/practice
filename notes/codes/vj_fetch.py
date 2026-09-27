import ssl, urllib.request
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE
req = urllib.request.Request("https://vjudge.net/article/11277", headers={"User-Agent":"Mozilla/5.0"})
try:
    with urllib.request.urlopen(req, timeout=25, context=ctx) as r:
        data = r.read().decode("utf-8", "ignore")
    print("HTTP OK, ?? =", len(data))
    open(r"D:\lenovo\chat_with_deepseek_harness\test_\vj2.html","w",encoding="utf-8").write(data)
except Exception as e:
    print("??:", e)
