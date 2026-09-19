#include<bits/stdc++.h>
using namespace std;

const double eps = 1e-4;
double a,b,c,d;

double f(double x){
    return a * x * x * x + b * x * x + c * x + d;
}

int main(){
    scanf("%lf%lf%lf%lf",&a,&b,&c,&d);
    for(int i = -100;i<=100;++i){
        double l = i,r = i + 1;
        if(fabs(f(l)) < eps) printf("%.2lf ",l);

        else if(fabs(f(r)) < eps) continue;

        else if(f(l) * f(r) < 0){
            while(abs(l - r) > eps){
                double mid = (l + r) / 2;
                if(f(mid) * f(r) > 0) r = mid;
                else l = mid;
            }
            printf("%.2lf ",l);
        }
    }

    return 0;
}