#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a = 0,b = 0,h = 0; cin>>a>>b>>h;
    int cnt = 0,s = 0;

    while(s < h){
        cnt++;
        
        s += a;
        if(s >= h) break; 
        s -= b;
        if(s < 0) s = 0;
    }

    cout<<cnt;

    return 0;
}