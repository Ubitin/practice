import fitz
p = r"D:\lenovo\chat_with_deepseek_harness\NWPUPC & NWPU-ATAS 2026.pdf"
d = fitz.open(p)
print("?? =", d.page_count)
out = []
for i, page in enumerate(d):
    t = page.get_text()
    out.append(f"\n===== ? {i+1} ? =====\n{t}")
txt = "".join(out)
open(r"D:\lenovo\chat_with_deepseek_harness\test_\nwpupc_text.txt", "w", encoding="utf-8").write(txt)
print("????? =", len(txt))
