#include<bits/stdc++.h>
using namespace std;

int sum(int n){
    int s =0;
    for(int i = 1;i<=n;++i) s += i;
    return s;
}

int cal(int n){
    int c = 0;
    for(int i = 1;i<=n;++i) c += i*i;
    return c;
}

int main(){
    int day,x,n;
    cin>>day;
    for(int i =1;;++i){
        if(day>=sum(i)&&day<sum(i+1)){
            n = i;
            break;
        }
    }
    x = day - sum(n);
    cout<<cal(n)+x*(n+1);


    return 0;    
}