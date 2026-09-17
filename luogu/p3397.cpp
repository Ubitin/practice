#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0, m = 0;
    cin>>n>>m;

    // 二维差分数组必须开 n+2：更新会写 d[x2+1][...]、d[...][y2+1]，
    // 地毯贴右/下边界时 x2+1、y2+1 取到 n+1；原版开 n+1 只到下标 n → 越界写 → WA
    // 用 vector 放堆上：n=1000 时两个 ~4MB 的 VLA 会栈溢出(0xC00000FD) → RE
    vector<vector<int>> d(n + 2, vector<int>(n + 2, 0));

    int x1,x2,y1,y2;
    for(int i = 0;i<m;++i){
        cin>>x1>>y1>>x2>>y2;

        d[x1][y1] += 1;
        d[x2 + 1][y1] -= 1;
        d[x1][y2 + 1] -= 1;
        d[x2 + 1][y2 + 1] += 1;
    }

    // 直接在 d 上做二维前缀和还原并输出（省掉额外 a 数组）
    for(int i = 1;i<=n;++i){
        for(int j = 1;j<=n;++j){
            d[i][j] += d[i - 1][j] + d[i][j - 1] - d[i - 1][j - 1];
            cout<<d[i][j]<<(j == n ? '\n' : ' ');
        }
    }
    return 0;
}