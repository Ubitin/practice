#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    string s; int len;
    cin>>len>>s;

    int cnt = 0;

    if(len <= 1){cout<<0; return 0;}

    for(int i = 0;i<len - 1;++i){
        if(s[i] == 'V'&&s[i+1] == 'K'){
            s[i] = 'X'; s[i+1] = 'X';
            cnt ++;
        }
    }

    for(int i = 0;i<len - 1;++i){
        if(s[i] == s[i+1] && s[i] != 'X'){
            cnt++; break;
        }
    }


    cout<<cnt;
    return 0;
}