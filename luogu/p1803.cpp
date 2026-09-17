#include<bits/stdc++.h>
using namespace std;

struct contest{
    long long l,r;
};

bool cmp(contest a,contest b){
    return a.r <= b.r;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n = 0; cin>>n;
    vector<contest>con(n);

    long long ans = 0,finish = 0;

    for(long long i = 0;i<n;++i) cin>>con[i].l>>con[i].r;

    sort(con.begin(),con.end(),cmp);

    for(long long i = 0;i<n;++i){
        if(con[i].l >= finish){
            ans ++; finish = con[i].r;
        }
    }

    cout<<ans;

    return 0;
}