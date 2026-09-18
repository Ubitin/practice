#include<bits/stdc++.h>
using namespace std;

string expand(){
    string s = "",x; char c; int d = 0;

    while(cin>>c){
        if(c == '['){
            cin>>d; x = expand();
            while(d--) s += x;
        }
        else if(c == ']') return s;

        else s += c;
    }

    return s;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout<<expand();

    return 0;
}