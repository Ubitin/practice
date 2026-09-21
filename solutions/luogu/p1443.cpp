#include<bits/stdc++.h>
using namespace std;

long long ans[401][401];

struct dot{//用一个结构体来存储每个点的横纵坐标
    long long x; long long y;
};

long long horse[8][2] = {{1,2},{1,-2},{-1,2},{-1,-2},
                    {2,1},{2,-1},{-2,1},{-2,-1}};//马可以走的8个方向

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    memset(ans,-1,sizeof(ans));//初始化，-1表示从未访问过

    queue<dot>q;//广度优先搜索的队列

    long long n,m; cin>>m>>n;
    dot temp; cin>>temp.x>>temp.y;
    q.push(temp); ans[temp.x][temp.y] = 0;

    while(!q.empty()){
        dot u = q.front(); q.pop();
        long long xu = u.x; long long yu = u.y;
        long long xn,yn;
        for(long long i = 0;i<8;++i){
            xn = xu + horse[i][0];  yn = yu + horse[i][1];
            
            if(xn <= 0 || xn > m || yn <= 0 || yn > n || ans[xn][yn] != -1) continue;
            else{
                dot d; d.x = xn; d.y = yn;
                ans[xn][yn] = ans[xu][yu] + 1;
                q.push(d);
            }
        }
    }

    for(long long i = 1;i<=m;++i){
        for(long long j = 1;j<=n;++j){
            cout<<ans[i][j]<<" ";
        }
        cout<<"\n";
    }

    return 0;
}

//此题积累的教训：一定要分清行和列，不要在写代码的中途改变，否则在遇到非正方形的矩形时会非常难办