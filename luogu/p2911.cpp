#include<bits/stdc++.h>
using namespace std;

struct bone{
    int val,num;
};

bool cmp(bone a,bone b){
    if(a.num != b.num) return a.num<b.num;
    else if(a.num == b.num){
        if(a.val != b.val) return a.val>b.val;
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int s1,s2,s3; cin>>s1>>s2>>s3;

    vector<bone>v;
    v.resize(s1+s2+s3+1);
    v[0].num = 0;v[1].num = 0;v[2].num = 0;
    v[0].val = 0;v[1].val = 1;v[2].val = 2;

    for(int i = 1;i<=s1;++i){
        for(int j = 1;j<=s2;++j){
            for(int k = 1;k<=s3;++k){
                v[i + j + k].num++; v[i + j + k].val = i + j + k;
            }
        }
    }

    sort(v.begin(),v.end(),cmp);

    cout<<v[s1 + s2 + s3].val;

    return 0;
}