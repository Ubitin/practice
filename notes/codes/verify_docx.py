import os, sys, zipfile

docx = sys.argv[1]
need = sys.argv[2:]

print('文件:', docx)
print('大小: %d 字节' % os.path.getsize(docx))
print('修改时间:', __import__('datetime').datetime.fromtimestamp(os.path.getmtime(docx)).strftime('%Y-%m-%d %H:%M:%S'))

with zipfile.ZipFile(docx) as z:
    xml = z.read('word/document.xml').decode('utf-8')

for s in need:
    print(('  含 %-46s : %s' % (s, 'OK' if s in xml else '缺失!')))

# 分页估计：统计分页符 / 段落数
print('段落数(w:p):', xml.count('<w:p '), '/', xml.count('<w:p>'))
