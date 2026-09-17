#include<bits/stdc++.h>
using namespace std;


int main(){
    
    ios::sync_with_stdio(false);

    string s;
    getline(cin,s);

    s.erase(remove(s.begin(),s.end(),' '),s.end());

    cout<<s;

    return 0;
}