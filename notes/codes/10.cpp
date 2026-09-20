#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100005;        // ① 常量：题目给的最大数据规模 + 余量
pair<int, int> a[MAXN];         // ② 全局数组：最多存 MAXN 个 (x,y) 点

int main() {
    // 典型用法：读入 n 个点对（n 一定 ≤ MAXN）
    int n;
    cin >> n;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i].first >> a[i].second;
    }
    // 按 first 排序（first 相同比 second）——pair 自带比较规则
    sort(a + 1, a + n + 1);
    for (int i = 1; i <= n; ++i)
        cout << a[i].first << "," << a[i].second << " ";
    cout << "\n";

    // ③ 内存占用（8 字节/个 pair<int,int>）
    cout << "sizeof(pair<int,int>) = " << sizeof(pair<int,int>) << " 字节\n";
    cout << "MAXN 个共 " << sizeof(a) / 1024 << " KB\n";
    return 0;
}
