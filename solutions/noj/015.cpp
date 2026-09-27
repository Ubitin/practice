#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s; long long n; long long cnt = 0;
    cin>>s;
    n = stoi(s);
    
    while(n > 0){
        s = to_string(n);
        long long lenth = s.size();
        char c; long long d = 0;
        for(long long i = 0;i<lenth;++i){
            c = s[i];
            d += c - '0';
        }
        n -= d;
        cnt ++;
    }

    cout<<cnt;

    return 0;
}