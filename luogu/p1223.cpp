#include<bits/stdc++.h>
using namespace std;

struct pe{ int num,t;};

bool cmp(pe x,pe y){
    return x.t < y.t;
}

int main(){
    int n; cin>>n;

    vector<pe>p; p.resize(n);

    for(int i = 0;i<n;++i) {
        cin>>p[i].t;
        p[i].num = i + 1;
    }
    sort(p.begin(),p.end(),cmp);

    long long sum = 0; double ans = 0;
    for(int i = 0;i<n;++i){
        sum += (n - i - 1) * p[i].t;
        cout<<p[i].num<<" ";
    }
    cout<<"\n";

    ans = 1.0 * sum / n;
    printf("%.2lf",ans);

    return 0;
}