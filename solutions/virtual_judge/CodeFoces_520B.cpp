#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e8;
ll n,m;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>n>>m;
    
    if(n >= m){
        cout<<n - m;
    }

    else if(n < m){//倒着推的贪心
        ll cnt = 0;
        while(m > n){
            if(m % 2 == 0){
                m /= 2;
                cnt ++;
            }
            else {
                m++; cnt++;
            }
        }
        cnt += (n - m);
        cout<<cnt;
    }

    return 0;
}