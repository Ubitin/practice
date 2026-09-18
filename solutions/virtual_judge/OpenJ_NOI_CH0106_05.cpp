#include<bits/stdc++.h>
using namespace std;

void print(int x,int n){
    long double t = (long double)x/(long double)n;
    cout<<fixed<<setprecision(2)<<t*100<<"%\n";
}

int main(){
    int n;
    cin>>n;
    vector<int> peo(n);
    for(int i =0;i<n;i++) cin>>peo[i];

    int arr[4]={0,0,0,0};
    for(size_t i =0;i<peo.size();++i)
    {
        if(peo[i]>=0&&peo[i]<=18) arr[0]++;
        else if(peo[i]>=19&&peo[i]<=35) arr[1]++;
        else if(peo[i]>=36&&peo[i]<=60) arr[2]++;
        else if(peo[i]>=61) arr[3]++;
    }
    
    for(int j =0;j<4;++j) print(arr[j],n);

    return 0;    
}