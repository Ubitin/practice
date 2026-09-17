#include<bits/stdc++.h>
using namespace std;
struct student
{
    int yw,sx,yy,zf;
    int id;
};
int sum(int a,int b,int c)
{
    return a + b + c;
}
bool f(struct student s1,struct student s2)
{
    if(s1.zf != s2.zf) return s1.zf>s2.zf;
    else if(s1.yw != s2.yw) return s1.yw>s2.yw;
    else return s1.id<s2.id;
}
int main(){
    int a,b,c,i,n;
    cin>>n;
    vector <struct student> s(n);
    for(int i = 0;i<n;++i)
    {
        cin>>s[i].yw;
        cin>>s[i].sx;
        cin>>s[i].yy;
        s[i].zf = sum(s[i].yw,s[i].sx,s[i].yy);
        s[i].id = i+1;
    }
    sort(s.begin(),s.end(),f);
    for(int j = 0;j<5;++j)
    {
        cout<<s[j].id<<" "<<s[j].zf<<endl;
    }
    return 0;
}