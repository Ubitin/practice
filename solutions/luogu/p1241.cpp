#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 105;
bool check[maxn]{false};//用于标记是否配对
stack<char> q; stack<ll>id;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s; cin>>s;
    for(size_t i = 0;i<s.size();++i){
        if(s[i] == '[' || s[i] == '(') {q.push(s[i]); id.push(i);}
        else {
            if(s[i] == ']') {
                if(!q.empty() && q.top() == '[') {
                    check[i] = 1; check[id.top()] = 1;
                    q.pop(); id.pop();
                }
            }

            else if(s[i] == ')') {
                if(!q.empty() && q.top() == '(') {
                    check[i] = 1; check[id.top()] = 1;
                    q.pop(); id.pop();
                }
            }
        }
    }

    for(size_t i = 0;i<s.size();++i){
        if(!check[i]) {
            if(s[i] == ')') cout<<'(';
            if(s[i] == ']') cout<<'[';
            cout<<s[i];
            if(s[i] == '[') cout<<']';
            if(s[i] == '(') cout<<')';
        }
        else cout<<s[i];
    }

    return 0;
}