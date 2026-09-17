# queue 详解：先进先出的"排队队列"

> 前置：STL 讲义 §7（queue 基础）、`算法讲义-02` §3.2（BFS 用队列）。`queue` 属于容器适配器，头文件 `<queue>`（`<bits/stdc++.h>` 已含）。
> 演示程序：`test_\12.cpp`（全部结论已用 g++ C++17 实测）。

---

## 0. 一句话本质

**queue = 先进先出（FIFO）的队列**——先入队的先被取出，就像食堂排队：先来的人先打饭。它是 BFS（广度优先搜索）的固定搭档。

## 1. 基本操作（全部 O(1)，背下来）

| 操作 | 含义 | 返回 |
|---|---|---|
| `q.push(x)` | 入队（加到队尾） | 无 |
| `q.pop()` | 出队（弹掉队首） | **无**（不返回值！） |
| `q.front()` | 看队首元素 | 引用 |
| `q.back()` | 看队尾元素 | 引用 |
| `q.size()` | 元素个数 | size_t |
| `q.empty()` | 是否为空 | bool |

```cpp
queue<int> q;
q.push(1); q.push(2); q.push(3);
q.front();   // 1
q.back();    // 3
q.pop();     // 弹掉 1
q.front();   // 2（实测）
```

## 2. 遍历的正确姿势：边弹边看

**queue 没有迭代器、不能下标访问**——想"看一遍"只能弹：

```cpp
while (!q.empty()) {
    cout << q.front() << " ";
    q.pop();          // 弹掉才能看到下一个
}
// 实测输出: 1 2 3 4 5（注意：遍历完队列就空了）
```

> 需要"看完还保留"？先拷贝一份副本再遍历，或者用 `deque`/`vector`。

## 3. 与 stack 对比（一张表记住两者）

| | stack（栈） | queue（队列） |
|---|---|---|
| 原则 | 后进先出 LIFO | 先进先出 FIFO |
| 类比 | 一摞盘子 | 排队打饭 |
| 取元素 | `top()` | `front()` / `back()` |
| 弹出 | `pop()` 不返回值 | `pop()` 不返回值 |
| 底层 | deque | deque |
| 典型应用 | 括号匹配、表达式求值 | BFS、模拟排队 |

## 4. 底层：deque（双端队列）

`queue` 默认用 **deque** 实现（两端都能进出的队列）。deque 自己也能直接用：

```cpp
deque<int> dq;
dq.push_back(1); dq.push_front(0);   // 两端都能加
dq.front(); dq.back();               // 0 1（实测）
```
deque 还支持下标访问（`dq[i]`）——queue 不行。需要"两端操作"时直接用 deque。

## 5. 经典应用：BFS（必会，图论入门钥匙）

**BFS = 用队列一层层往外扩**，首次到达某点就是最短步数（无权图）：

```cpp
// 从 1 出发，可走 +1 或 ×2，求到 5 的最少步数（实测答案 3：1→2→4→5）
queue<int> bfs;
vector<int> step(20, -1);      // -1 = 未访问
bfs.push(1); step[1] = 0;
while (!bfs.empty()) {
    int cur = bfs.front(); bfs.pop();
    if (cur == target) break;
    for (int nxt : {cur + 1, cur * 2}) {
        if (step[nxt] == -1) {
            step[nxt] = step[cur] + 1;   // 由近及远，第一次到达即最短
            bfs.push(nxt);
        }
    }
}
```

> 对应洛谷：P1996 约瑟夫问题（模拟出队）、P1540 机器翻译（内存队列）、P1443 马的遍历（BFS 最短路）、P5318 查找文献（BFS+DFS）。

## 6. 常见坑（四个，全踩过一遍）

1. **`pop()` 不返回被弹的元素**——要先 `front()` 取值再 `pop()`：
```cpp
int x = q.front(); q.pop();   // ✅ 正确顺序
int y = q.pop();              // ❌ 编译错误
```
2. **空队列取 `front()`/`back()` 是未定义行为**（可能崩溃）——取之前必须判空：
```cpp
if (!q.empty()) { cout << q.front(); }
```
3. **`queue` 没有 `clear()`**——清空要么循环弹（O(n)），要么直接换新队列（O(1)，竞赛常用）：
```cpp
q = queue<int>();     // 赋一个空队列，原内容自动释放
```
4. **不能随机访问、不能迭代器遍历**——有这需求换 `deque`/`vector`。

## 7. 课程视角：C 语言数组模拟队列（期末考点）

C 语言没有现成 queue，用数组模拟（循环队列，判满 `(rear+1)%MAXSIZE==front`）：

```c
int q[1005], head = 0, tail = 0;        // [head, tail) 是有效区间
q[tail++] = 1;                          // 入队
int x = q[head++];                      // 出队
```
> 理解这个手写版，再看 C++ 的 `queue` 就是"别人帮你写好的循环队列"。

## 8. queue 与 priority_queue 别混

- `queue`：严格 FIFO，谁先进先出。
- `priority_queue`：**优先队列 = 堆**，每次取"最大/最小"，不是 FIFO！两者完全不同的容器适配器，别用错。

## 9. 习题（答案已实测）

**E1** 写出输出：
```cpp
queue<int> q;
q.push(1); q.push(2); q.pop(); q.push(3);
cout << q.front() << " " << q.back();
```

**E2** 判断对错：
```cpp
A. queue 有 clear() 方法
B. q.pop() 返回被弹出的元素
C. queue 可以用迭代器遍历
D. 空队列调用 q.front() 是未定义行为
```

**E3** 补全：遍历并打印队列所有元素（打印后队列为空）：
```cpp
while (____) { cout << ____ << " "; ____; }
```

**E4** 读程序写输出：
```cpp
queue<int> q;
for (int i = 1; i <= 3; i++) q.push(i * 10);
q.pop(); q.push(40);
while (!q.empty()) { cout << q.front() << " "; q.pop(); }
```

**E5** 设计题：约瑟夫问题，n 个人围一圈从 1 报数，报到 3 的出列——用 queue 模拟的核心思路是什么？（提示：不是报到 3 的怎么办？）

---

## 答案与解析

**E1** → `2 3`
push 1,2 → pop 弹掉 1 → push 3 → 队里是 2,3 → front=2, back=3。

**E2** → **D 对；A、B、C 错**。queue 无 clear；pop 无返回值；无迭代器；空队列 front 是 UB（可能崩溃）。

**E3** →
```cpp
while (!q.empty()) { cout << q.front() << " "; q.pop(); }
```

**E4** → `20 30 40`
入队 10,20,30 → pop 弹 10 → push 40 → 队列 20,30,40 → 依次弹出。

**E5** → 核心思路：**循环"报数-转移"**——队首不是 3 的人，从队首弹出再放回队尾（转一圈继续报）；报到 3 的直接弹出不再放回。即：
```cpp
while (q.size() > 1) {
    for (int i = 1; i < 3; i++) { q.push(q.front()); q.pop(); }  // 前两个转移到队尾
    q.pop();                                                      // 第 3 个出列
}
```
（P1996 的解法骨架，用 queue 模拟"转圈"。）

---

**一句话总结**：queue = **FIFO + 四个操作（push/pop/front/back）+ 判空**——没有迭代器、pop 不返回值、取前必判空；它是 BFS 的固定搭档，约瑟夫/排队/消息队列都是它的主场。
