import glob, os, re, zipfile
d = r"D:\lenovo\chat_with_deepseek_harness\??"
p = [f for f in glob.glob(os.path.join(d,"*.docx"))][0]
xml = zipfile.ZipFile(p).read("word/document.xml").decode("utf-8")
# ??????
styles = re.findall(r'w:pStyle w:val="([^"]+)"', xml)
from collections import Counter
print("docx ??????:", Counter(styles).most_common(8))
# ????
text = re.sub(r"<[^>]+>", "", xml)
print("??? =", len(text))
print("? '6.16' =", "6.16" in text, " ? '3.17' =", "3.17" in text, " ? '????' =", "????" in text)
