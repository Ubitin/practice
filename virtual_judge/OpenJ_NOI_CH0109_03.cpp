#include<bits/stdc++.h>
using namespace std;
struct time
{
    int am,bm,sum;
    int day;
};

bool f(struct time t1,struct time t2)
{
    if(t1.sum!=t2.sum)
    {
        return t1.sum>t2.sum;
    }
    else return t1.day<t2.day;
}

int main(){
    vector<struct time> t(7);
    int temp1,temp2;
    int i;
    while(cin>>temp1,cin>>temp2)
    {
        t[i]={temp1,temp2,temp1+temp2,i+1};
        ++i;
    }
    sort(t.begin(),t.end(),f);
    if(t[0].sum<=8)
    {
        cout<<0;
    }
    else cout<<t[0].day;
    return 0;
}