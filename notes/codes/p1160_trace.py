# -*- coding: utf-8 -*-
# 逐条追踪：模拟"数组链表"与 std::list 两种实现，找出第一个分叉的操作
import random

def gen(seed):
    rng = random.Random(seed)
    n = rng.randint(1, 25)
    alive = [1]; ins = []
    for i in range(2, n + 1):
        k = rng.choice(alive); p = rng.choice([0, 1])
        ins.append((k, p)); alive.append(i)
    m = rng.randint(0, max(0, n - 1))
    dels = rng.sample(alive, m) if m else []
    return n, ins, dels


def sim_stdlib(n, ins, dels):
    """用 Python list 模拟 std::list：insert 在 k 左边/右边"""
    L = [1]
    for idx, (k, p) in enumerate(ins):
        i = idx + 2
        j = L.index(k)
        if p == 0: L.insert(j, i)       # 插到 k 左边（k 及其右侧后移）
        else:      L.insert(j + 1, i)   # 插到 k 右边
    alive = [x for x in L if x not in set(dels)]
    return alive


def sim_lazy(n, ins, dels):
    """惰性删除：链上保留，输出时跳过——等价于 stdlib 模拟但不真删"""
    L = [1]
    for idx, (k, p) in enumerate(ins):
        i = idx + 2
        j = L.index(k)
        if p == 0: L.insert(j, i)
        else:      L.insert(j + 1, i)
    return [x for x in L if x not in set(dels)]


# 用 seed=0 的数据，打印每个插入步骤后的队列
n, ins, dels = gen(0)
print("n =", n, " 删除 =", dels)
L = [1]
print("初始:      ", L)
for idx, (k, p) in enumerate(ins):
    i = idx + 2
    j = L.index(k)
    if p == 0: L.insert(j, i)
    else:      L.insert(j + 1, i)
    print("插入 %2d 到 %2d %s -> %s" % (i, k, "左" if p == 0 else "右", L))
print("\n删除后:", [x for x in L if x not in set(dels)])

# 关键检验：每一步"插入后"的队列，是否与"数组链表"实现一致？
# 数组链表实现：新节点接在 now 的前/后
class ArrList:
    def __init__(self):
        self.pre = {0: 0}; self.nxt = {0: 0}; self.key = {}
        self.idx = {}; self.tot = 0
        self.add(1)
    def add(self, key):
        self.tot += 1
        t = self.tot
        self.pre[t] = 0; self.nxt[t] = 0; self.key[t] = key
        self.idx[key] = t
        return t
    def ins_back(self, x, y):
        now = self.idx[x]; t = self.add(y)
        self.pre[t] = now; self.nxt[t] = self.nxt[now]
        self.pre[self.nxt[now]] = t          # 注意：若 nxt[now]=0（哨兵），这会把 pre[0] 改掉
        self.nxt[now] = t
    def ins_front(self, x, y):
        now = self.idx[x]; t = self.add(y)
        self.pre[t] = self.pre[now]; self.nxt[t] = now
        self.nxt[self.pre[now]] = t
        self.pre[now] = t
    def walk(self):
        out = []; cur = 1 if 1 in self.idx else 0
        # 从编号1的节点开始（若存在）
        cur = self.idx.get(1, self.nxt[0])
        guard = 0
        while cur and guard < 100:
            out.append(self.key[cur]); cur = self.nxt[cur]; guard += 1
        return out

A = ArrList()
print("\n数组链表逐条推演（从编号1开始走）:")
for idx, (k, p) in enumerate(ins):
    i = idx + 2
    if p == 0: A.ins_front(k, i)
    else:      A.ins_back(k, i)
    print("插入 %2d 到 %2d %s -> %s" % (i, k, "左" if p == 0 else "右", A.walk()))
