#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    priority_queue<int,vector<int>,greater<int>>apple;
    int t = 0; cin>>t;

    while(t--){
        int x = 0; cin>>x;
        apple.push(x);
    }

    int ans = 0;
    while(apple.size() >= 2){
        int temp = 0;
        temp += apple.top(); apple.pop();
        temp += apple.top(); apple.pop();
        ans += temp;
        apple.push(temp);
    }

    cout<<ans;

    return 0;
}