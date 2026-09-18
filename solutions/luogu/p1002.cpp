#include<bits/stdc++.h>
using namespace std;
const int MAXN = 22;

long long f[MAXN][MAXN] = {0};
int ctrl[MAXN][MAXN] = {0};

int d[9][2] = {{0,0},{1,2},{-1,-2},{-1,2},{1,-2},
                {2,1},{-2,-1},{-2,1},{2,-1}};



int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m,n,hx,hy; cin>>n>>m>>hx>>hy;

    for(int i = 0;i<9;++i){
        int x = hx + d[i][0]; int y = hy + d[i][1];

        if(x >= 0&&x <= n&&y >= 0 && y <= m) ctrl[x][y] = 1;
    }

    f[0][0] = 1 - ctrl[0][0];

    for(int i = 0;i<=n;++i){
        for(int j = 0;j<=m;++j){
            if(ctrl[i][j]) continue;

            if(i) f[i][j] += f[i-1][j];
            if(j) f[i][j] += f[i][j-1];
        }
    }

    cout<<f[n][m]<<"\n";

    return 0;
}