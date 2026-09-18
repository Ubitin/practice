#include<bits/stdc++.h>
using namespace std;

//判断是否存在该数字
bool has(string s,char b){
    if(s.find(b) != string::npos) return true;
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int len = 0; cin>>len;

    string num; cin>>num;

    if(has(num,'0')){
        if(has(num,'1') || has(num,'2') || has(num,'3')) cout<<"YES\n";
        else cout<<"NO\n";
    }

    else{
        int cnt = 0;
        if(has(num,'1') || has(num,'4') || has(num,'7')) cnt++;
        if(has(num,'3') || has(num,'6') || has(num,'9')) cnt++;
        if(has(num,'1') || has(num,'2') || has(num,'3')) cnt++;
        if(has(num,'7') || has(num,'9')) cnt++;

        if(cnt == 4) cout<<"YES\n";
        else cout<<"NO\n";
    }

    return 0;
}