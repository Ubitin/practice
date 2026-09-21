#include<bits/stdc++.h>
using namespace std;

int n,a,b;
int ans[210],k[210];//到达每层楼最少所需按的次数，每层楼可以移动的层数

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>a>>b;
    queue<int>q;//广度优先搜索的队列
    q.push(a);
    for(int i = 1;i<=n;++i) cin>>k[i];
    memset(ans,-1,sizeof(ans));//初始化，-1表示从未访问过
    ans[a] = 0;

    while(!q.empty()){
        int temp = q.front(); q.pop();

        if(temp == b) break;//如果到达b层了，停止搜索
        if(temp + k[temp] <= n){
            if(ans[temp + k[temp]] == -1){
                int d = temp + k[temp];
                q.push(d);
                ans[d] = ans[temp] + 1;
            }
        }

        if(temp - k[temp] >= 1){
            if(ans[temp - k[temp]] == -1){
                int d = temp - k[temp];
                q.push(d);
                ans[d] = ans[temp] + 1;
            }
        }
    }

    cout<<ans[b];

    return 0;
}