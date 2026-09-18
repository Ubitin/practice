#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,s; cin>>n>>s;

    int a,b; cin>>a>>b; int h = a + b;

    priority_queue<int,vector<int>,greater<int>>app;

    for(int i = 0;i<n;++i){
        int x,y; cin>>x>>y;
        if(x > h) continue;
        else if(x <= h) app.push(y);
    }

    int ans = 0;
    while(s){
        int temp = app.top();
        if(temp <= s){
            s -= temp; ans ++;
            app.pop();
        }
        else break;
    }

    cout<<ans;

    return 0;
}