#include<bits/stdc++.h>
using namespace std;
struct coin{
    int m,v;
};

bool cmp(coin a,coin b){
    return a.v * b.m > a.m * b.v;
}

int main(){
    int n,t; vector<coin>a;
    scanf("%d%d",&n,&t);
    a.resize(n);

    int c = t;float val = 0;

    for(int k = 0;k<n;++k){
        scanf("%d%d",&a[k].m,&a[k].v);
    }

    sort(a.begin(),a.end(),cmp);

    int i;

    for(i = 0;i<n;++i){
        if(a[i].m > c) break;

        c -= a[i].m; val += a[i].v;
    }

    if(i < n) val += (1.0 * c * a[i].v) / a[i].m; 

    printf("%.2lf",val);

    return 0;
}