#include<bits/stdc++.h>
using namespace std;
int t[10];

void f1(long long x){
    t[x%10] = 1;//个位数
    t[x/10%10] = 1;//十位数
    t[x/100] = 1;
}

bool f2(long long x,long long y,long long z){
    memset(t,0,sizeof(t));
    f1(x),f1(y),f1(z);
    for(int i = 1;i<=9;++i){
        if(!t[i]){return 0; break;}
        
    }
    return 1;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long x,y,z,a,b,c,cnt;
    cnt = 0;
    cin>>a>>b>>c;
    if(a == 0||b == 0||c == 0){cout<<"No!!!"<<"\n"; return 0;}

    for(x=123;x<=987;++x){
        if(x*b%a||x*c%a) continue;
        else{
            y = x*b/a; z = x*c/a;
            if(y>999||z>999) continue;
            else{
                if(f2(x,y,z)){cout<<x<<" "<<y<<" "<<z<<"\n"; cnt++;}
            }
        }
    }
    if(!cnt) cout<<"No!!!"<<"\n";

    return 0;
}