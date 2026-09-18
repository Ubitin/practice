#include<bits/stdc++.h>
using namespace std;
int main(){
    int m,n;
    cin>>m>>n;

    vector<int>a;
    a.resize(10);
    
    string s;
    for(int i = m;i<=n;++i){
        s = to_string(i);
        for(size_t i = 0;i<s.size();i++){
            int x = s[i] - '0'; a[x]++;
        }
    }

    for(int i = 0;i<10;++i) cout<<a[i]<<" ";

    return 0 ;
}