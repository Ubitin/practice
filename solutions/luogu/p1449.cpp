#include<bits/stdc++.h>
using namespace std;
using ll = long long;
string num; stack<ll>s;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    char c; ll temp = 0;
    do{
        c = getchar();
        if(c >= '0' && c <= '9') temp = temp * 10 + c - '0';
        else if(c == '.'){
            s.push(temp); temp = 0;
        }
        else if(c != '@') {
            ll x = s.top(); s.pop();
            ll y = s.top(); s.pop();

            switch (c){
                case '+' : s.push(x + y); break;
                case '-' : s.push(y - x); break;
                case '*' : s.push(x * y); break;
                case '/' : s.push(y / x); break;
                default : break;
            }
        }
    }while(c != '@');
    cout<<s.top();

    return 0;
}