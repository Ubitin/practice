#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 2e4 + 5;
ll n,m;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    
    if(n >= m){
        cout<<n - m;
    }

    else if(n < m) {//bfs
        ll dist[maxn]; memset(dist,-1,sizeof(dist));
        queue<ll>q; q.push(n);
        dist[n] = 0;
        while(!q.empty()){
            ll u = q.front(); q.pop();
            if(u == m) break;
            else {
                ll v[2] = {u - 1, u * 2};
                for(int i = 0;i<2;++i){
                    if(v[i] < 1 || v[i] > maxn || dist[v[i]] != -1) continue;

                    else {
                        dist[v[i]] = dist[u] + 1;
                        q.push(v[i]);
                    }
                }
            }
        }

        cout<<dist[m];
    }

        

    return 0;
}