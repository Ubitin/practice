#include<bits/stdc++.h>
using namespace std;
struct can{
    int num; string a;
};

bool cmp(can s1,can s2){
    if(s1.a.size() != s2.a.size()) return s1.a.size()>s2.a.size();
    else if(s1.a.size() == s2.a.size()){
        for(size_t i = 0;i<s1.a.size();++i){
            if((s1.a[i] - '0') != (s2.a[i] - '0')){
                return s1.a[i] >s2.a[i]; break;
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 0;cin>>n;

    vector<can>u; u.resize(n);

    for(int i = 0;i<n;++i){
        cin>>u[i].a; u[i].num = i+1;
    }

    sort(u.begin(),u.end(),cmp);

    cout<<u[0].num<<"\n"<<u[0].a;

    return 0;
}