#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n = 0,k = 0;
    cin>>n>>k;

    priority_queue<int,vector<int>,greater<int>>qmin;
    
    for(int i = 0;i<n;i++){
        int x; cin>>x;
        if(i<=k - 1) qmin.push(x);
        else{
            qmin.push(x);
            qmin.pop();
        }
        if(i>=k-1) cout<<qmin.top()<<"\n";
    }

    return 0;
}