#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> course(n);
    for (int i = 0; i < n; ++i) cin >> course[i];

    int m;
    cin >> m;
    map<int, int> last;                    // 课程编号 → 最终成绩（后面的录入覆盖前面的）
    for (int i = 0; i < m; ++i) {
        int id, score;
        cin >> id >> score;
        last[id] = score;                  // map 的 m[key] 特性：自动插入/覆盖
    }

    int qk = 0, jg = 0;
    for (int id : course) {
        if (!last.count(id)) qk++;         // 缺课：没有任何录入
        else if (last[id] < 60) jg++;      // 不及格：最终成绩 < 60
    }
    cout << qk << " " << jg << "\n";
    return 0;
}
