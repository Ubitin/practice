#include<bits/stdc++.h>
using namespace std;

int num(char c){
    return c - '0';
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s; cin>>s;
    int k = 0; cin>>k;
    size_t n = s.size();

    for(auto iter = s.begin();k > 0;){
        if(iter == s.begin()){
            if(num(*iter) > num(*next(iter))){
                s.erase(s.begin(),s.begin() + 1);
                iter--;
                k--;
            }
            else iter++;
        }
        else if(iter == (s.begin() + n - 1)){
            if(num(*iter) > num(*prev(iter))){
                s.erase(s.begin() + n - 1,s.end());
                iter--;
                k--;
            }
            else iter++;
        }

        else{
            if(num(*iter > num(*prev(iter))) && num(*iter) > num(*next(iter))){
                s.erase(iter,next(iter));
                iter--;
                k--;
            }
            else iter++;
        }
    }

    size_t t = s.find_first_not_of('0');
    if(t == string::npos) s = "0";
    else s = s.substr(t);

    cout<<s;

    return 0;
}