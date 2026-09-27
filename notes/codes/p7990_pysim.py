a=[11,0,13]
ops=[("2",15),("1",2,15),("2",7),("1",2,9),("2",11),("2",8)]
for op in ops:
    if op[0]=="1":
        _,p,x=op; a[p-1]=x
    else:
        _,x=op
        for i in range(3):
            if a[i]<x: a[i]=x
    print(op, "->", a)
print("Python ??????:", a)
