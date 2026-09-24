#include<bits/stdc++.h>
using namespace std;
int r,g,b;
double v,s,h;

inline unsigned char lowByte(long long v) {
    return (unsigned char)(v & 0xFF);
}

int main(){
    cin>>r>>g>>b;
    r = lowByte(r); g = lowByte(g); b = lowByte(b);

    int M = max(max(r,g),max(g,b)); int m = min(min(r,g),min(g,b)); int d = M - m; 
    v = M;
    
    if(v == 0) s = 0;
    else s = d / M;

    if(M == r) h = 60 * ((g - b) / d);
    else if(M == g) h = 60 * (2 + (b - r) / d);
    else if(M == b) h = 60 * (4 + (r - g) / d);
    if(h < 0) h += 360;

    s *= 100; v *= 100;
    printf("%.4lf,%.4lf\%,%.4lf\%",h,s,v);
    return 0;
}