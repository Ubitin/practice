#include<bits/stdc++.h>
using namespace std;
using ll = long long;
string s1,s2; ll x1,dy1,x2,y2;

ll gcd_(ll x,ll y){
    while(y){
        ll t = x % y;
        x = y;
        y = t;
    }
    return x;
}

string simple(ll x,ll y){
    ll r = gcd_(x,y);
    x /= r; y /= r;

    if(x == 0) return "0";
    else {
        return to_string(x) + "/" + to_string(y);
    }
}


int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin>>s1>>s2;
    string mom1,son1,mom2,son2;

    ll pos1 = s1.find('/'); ll pos2 = s2.find('/');

    son1 = s1.substr(0,pos1); mom1 = s1.substr(pos1 + 1);
    son2 = s2.substr(0,pos2); mom2 = s2.substr(pos2 + 1);
    x1 = stoll(son1); dy1 = stoll(mom1);
    x2 = stoll(son2); y2 = stoll(mom2);

    cout<<"("<<x1<<"/"<<dy1<<")+("<<x2<<"/"<<y2<<")="<<simple(x1 * y2 + x2 * dy1,dy1 * y2)<<"\n";
    cout<<"("<<x1<<"/"<<dy1<<")-("<<x2<<"/"<<y2<<")="<<simple(x1 * y2 - x2 * dy1,dy1 * y2)<<"\n";
    cout<<"("<<x1<<"/"<<dy1<<")*("<<x2<<"/"<<y2<<")="<<simple(x1 * x2,dy1 * y2)<<"\n";
    cout<<"("<<x1<<"/"<<dy1<<")/("<<x2<<"/"<<y2<<")="<<simple(x1 * y2,x2 * dy1)<<"\n";

    return 0;
}