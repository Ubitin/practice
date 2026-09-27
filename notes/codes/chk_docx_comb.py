# -*- coding: utf-8 -*-
# 校验打印版 docx 里确实包含新小节（组合数）
import glob, os, re, zipfile

d = r"D:\lenovo\chat_with_deepseek_harness\讲义"
cands = [f for f in glob.glob(os.path.join(d, "*.docx"))]
p = cands[0]
print("docx =", os.path.basename(p), os.path.getsize(p), "bytes")

z = zipfile.ZipFile(p)
xml = z.read("word/document.xml").decode("utf-8")
text = re.sub(r"<[^>]+>", "", xml)
print("正文字符数 =", len(text))

keys = ["组合数", "C_iter", "逆元", "质因数", "6.16", "快速幂", "Lucas", "C(66,33)"]
for k in keys:
    print("  %-10s 出现 %d 次" % (k, text.count(k)))

i = text.find("组合数")
if i >= 0:
    print("\n[组合数] 首次出现处上下文：")
    print("  " + text[max(0, i - 80): i + 200].replace("\n", " "))
else:
    print("\n⚠️ docx 里没有『组合数』，说明重建没生效")
