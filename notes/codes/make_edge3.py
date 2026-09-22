import sys

# 造 edge3：进位链专项（300 个 1）+ 边界查询
lines = []
qs = []
for _ in range(300):
    qs.append("1 0")
for w in [1, 2, 3, 255, 256, 299, 300, 301, 512, 536870912, 1000000000]:
    qs.append("2 %d" % w)
lines.append(str(len(qs)))
lines.extend(qs)
open(sys.argv[1], 'w', encoding='ascii', newline='\n').write("\n".join(lines) + "\n")
print("edge3 写出:", sys.argv[1], "查询条数:", len(qs))
