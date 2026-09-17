#include<bits/stdc++.h>
using namespace std;

int j(string s){
    if(s=="y") return true;
    else return false;
}

int main(){
    int wei; string s;
    cin>>wei>>s;
    if(wei<=1000) cout<<8+j(s)*5;
    else if(wei>1000){
        int sy = wei - 1000;
        int n;
        if(sy%500 == 0) n = sy/500;
        else n = sy/500 +1;
        cout<<8+n*4+j(s)*5;
    }

    return 0;    
}