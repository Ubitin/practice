#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0, k = 0;
    cin >> n >> k;

    priority_queue<int, vector<int>, greater<int>> pq;  // 小根堆：堆顶=最小=当前第K大

    for (int i = 0; i < n; ++i) {
        int x; cin >> x;
        if (i < k) {
            pq.push(x);                 // 前 K 个直接进
        } else {
            pq.push(x);                 // 新元素进来
            pq.pop();                   // 丢掉最小的，堆保持 K 个
        }
        if (i >= k - 1) cout << pq.top() << "\n";   // 从第 K 个起每次输出堆顶
    }

    return 0;
}
