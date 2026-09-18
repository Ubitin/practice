#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d; cin>>d;

    if(d == 1) cout<<sizeof(char)<<","<<(long long)numeric_limits<char>::min()<<","<<(long long)numeric_limits<char>::max();

    else if(d == 2) cout<<sizeof(unsigned char)<<","<<(long long)numeric_limits<unsigned char>::min()<<","<<(long long)numeric_limits<unsigned char>::max();

    else if(d == 3) cout<<sizeof(short)<<","<<(long long)numeric_limits<short>::min()<<","<<(long long)numeric_limits<short>::max();

    else if(d == 4) cout<<sizeof(unsigned short)<<","<<(long long)numeric_limits<unsigned short>::min()<<(long long)numeric_limits<unsigned short>::max();

    else if(d == 5) cout<<sizeof(int)<<","<<(long long)numeric_limits<int>::min()<<","<<(long long)numeric_limits<int>::max();

    else if(d == 6) cout<<sizeof(unsigned int)<<","<<(long long)numeric_limits<unsigned int>::min()<<","<<(long long)numeric_limits<unsigned int>::max();

    else if(d == 7) cout<<sizeof(long)<<","<<(long long)numeric_limits<long>::min()<<","<<(long long)numeric_limits<long>::max();

    else if(d == 8) cout<<sizeof(unsigned long)<<","<<(long long)numeric_limits<unsigned long>::min()<<","<<(long long)numeric_limits<unsigned long>::max();
    
    else if(d == 9) cout<<sizeof(long long)<<","<<(long long)numeric_limits<long long>::min()<<","<<(long long)numeric_limits<long long>::max();

    else if(d == 10) cout<<sizeof(unsigned long long)<<","<<(unsigned long long)numeric_limits<unsigned long long>::min()<<","<<(unsigned long long)numeric_limits<unsigned long long>::max();

    return 0;
}