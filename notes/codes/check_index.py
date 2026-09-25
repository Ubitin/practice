# 索引完整性检查：索引里登记的行 vs 讲义文件夹里的真实文件
import io, os, re

D = r'D:\lenovo\chat_with_deepseek_harness\讲义'
IDX = os.path.join(D, '00-知识索引与复习路线图.md')
SKIP = {'00-知识索引与复习路线图.md', '机试代码模板速查手册.md'}

s = io.open(IDX, encoding='utf-8').read()
lines = s.split('\n')

rows = []
for no, ln in enumerate(lines, 1):
    m = re.match(r'^\|\s*`([^`]+)`\s*\|', ln)
    if m:
        rows.append((no, m.group(1)))

names = [n for _, n in rows]
files = {f[:-3] for f in os.listdir(D) if f.endswith('.md') and f not in SKIP}

missing_file = [(no, n) for no, n in rows if n not in files]
no_row = sorted(files - set(names))
dup = sorted({n for n in names if names.count(n) > 1})

print('索引里登记的讲义行数 =', len(rows))
print('讲义文件夹里真实 .md 数（去掉索引与手册）=', len(files))
print('登记了但文件不存在 =', len(missing_file))
for no, n in missing_file:
    print('   L%d: %s' % (no, n))
print('有文件但索引里没有行 =', len(no_row))
for n in no_row:
    print('   ', n)
print('重复登记 =', dup if dup else '无')
