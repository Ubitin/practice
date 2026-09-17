#include<bits/stdc++.h>
using namespace std;

double hav(double x){
    return (1 - cos(x)) / 2;
}

int main(){
    const double pi = acos(-1.0);

    double a1,b1,a2,b2; cin>>a1>>b1>>a2>>b2;
    a1 *= pi/180.0;
    b1 *= pi/180.0;
    a2 *= pi/180.0;
    b2 *= pi/180.0;

    double t = 0.0; int r = 6371;

    double temp = hav(a2 - a1) + cos(a1) * cos(a2) * hav(b2 - b1);

    t = acos(1 - 2 * temp);

    double d = (double)r * t;

    printf("%.4lfkm",d);

    return 0;
}