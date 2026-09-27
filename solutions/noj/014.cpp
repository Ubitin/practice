#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    bool ok = 1;
    string num; cin>>num;
    string res = num;

    long long lenth = num.size();

    for(long long i = 0;i<lenth;++i){
        char temp; temp = num[i];
        long long x = temp - '0';
        if(x == 3 || x== 4 || x == 7) ok = 0;
        else if(x == 2) num[i] = '5';
        else if(x == 5) num[i] = '2';
        else if(x == 6) num[i] = '9';
        else if(x == 9) num[i] = '6';
    }

    reverse(num.begin(),num.end());
    if(num != res) ok = 0;

    cout<<(ok ? "Yes" : "No");

    return 0;
}