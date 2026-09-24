#include<bits/stdc++.h>
using namespace std;

int main(){
    string num; cin>>num;
    long long lenth = num.size();

    long long pos = num.find('.');

    num.erase(num.begin() + pos,num.begin() + pos + 1);

    long long x = stoi(num);

    long long a = lenth - pos - 1;

    long long y = (long long)pow(10,a);
    long long m = min(y,x);

    for(long long i = m;i >= 2;--i){
        if((y % i == 0) && (x % i == 0)){
            y /= i; x /= i;
            break;
        }
    }
    printf("%lld/%lld",x,y);
    return 0;
}