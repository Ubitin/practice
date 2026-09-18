#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n,b; cin>>n>>b;

    vector<int>h;
    h.resize(n);
    for(int i = 0;i<n;++i) cin>>h[i];

    sort(h.begin(),h.end(),greater<int>());

    int sum = 0;
    for(int i = 0;i<n;i++){
        
        sum += h[i];

        if(sum >= b){
            cout<<i+1;
            break;
        }
    }

    return 0;
}