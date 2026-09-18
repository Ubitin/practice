#include<bits/stdc++.h>
using namespace std;
struct dot{
    int x,y;
    char id;
};

bool cmp(struct dot d1,struct dot d2){
    if(d1.y != d2.y) return d1.y < d2.y;
    else{
        if(d1.x != d2.x) return d1.x < d2.x;
    }
}

int main(){
    ios::sync_with_stdio(false);
    
    size_t n; cin>>n;

    vector<struct dot>v;
    v.resize(n);
    for(size_t i = 0;i<n;++i) cin>>v[i].x>>v[i].y;

    string s; s.resize(n);
    cin>>s;
    for(size_t i = 0;i<n;++i) v[i].id = s[i];

    sort(v.begin(),v.end(),cmp);

    bool a = 0;

    for(size_t i = 0;i<n - 1;++i){
        if(v[i].y == v[i+1].y&&v[i].id =='R'&&v[i + 1].id =='L'){
            a = 1; break;
        }
    }

    if(a == 0) cout<<"No";
    else cout<<"Yes";

    return 0;
}