#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. 基本操作：FIFO（先进先出）=====
    queue<int> q;
    q.push(1); q.push(2); q.push(3);     // 入队 1,2,3
    cout << "front=" << q.front() << " back=" << q.back()
         << " size=" << q.size() << " empty=" << q.empty() << "\n";
    q.pop();                              // 弹出队首 1
    cout << "pop 后 front=" << q.front() << "\n";

    // ===== 2. 遍历的正确姿势：边弹边看（queue 不支持迭代器！）=====
    queue<int> t;
    for (int i = 1; i <= 5; i++) t.push(i);
    cout << "遍历(边弹边看): ";
    while (!t.empty()) {
        cout << t.front() << " ";
        t.pop();                          // 弹掉才能看到下一个
    }
    cout << "\n";

    // ===== 3. 判空陷阱 =====
    queue<int> emptyQ;
    // emptyQ.front();  ← 空队列取 front 是未定义行为（崩溃）
    if (!emptyQ.empty()) cout << emptyQ.front() << "\n";

    // ===== 4. "清空"：queue 没有 clear() =====
    queue<int> big;
    for (int i = 0; i < 100; i++) big.push(i);
    // 方法一：循环弹空（O(n)）
    while (!big.empty()) big.pop();
    // 方法二：直接换一个新的空队列（O(1)，竞赛常用）
    big = queue<int>();
    cout << "清空后 size=" << big.size() << "\n";

    // ===== 5. 经典应用：BFS（层序遍历一棵"数字树"）=====
    // 从 1 开始，每个数 x 可以走到 x+1 和 x*2，求到 5 的最少步数
    int target = 5;
    queue<int> bfs;
    vector<int> step(20, -1);
    bfs.push(1); step[1] = 0;
    while (!bfs.empty()) {
        int cur = bfs.front(); bfs.pop();
        if (cur == target) break;
        for (int nxt : {cur + 1, cur * 2}) {
            if (nxt <= 15 && step[nxt] == -1) {
                step[nxt] = step[cur] + 1;
                bfs.push(nxt);
            }
        }
    }
    cout << "BFS: 1 到 " << target << " 的最少步数 = " << step[target] << "\n";

    // ===== 6. 底层与 deque 的关系 =====
    deque<int> dq;
    dq.push_back(1); dq.push_front(0);     // 两端都能加
    cout << "deque: front=" << dq.front() << " back=" << dq.back() << "\n";
    return 0;
}
