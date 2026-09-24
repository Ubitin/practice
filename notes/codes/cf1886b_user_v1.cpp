#include<bits/stdc++.h>
using namespace std;
const double del = 1e-11;
using ll = long long;
ll px,py,ax,ay,bx,by;

double dis(double x1,double y1,double x2,double y2){//计算两点直线距离
    double temp = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    double distance = sqrt(temp);
    return distance;
}

bool p(double w){
    if((ax * ax + ay * ay) > (w + del) && (bx * bx + by * by) > (w + del)) return false;
    if(dis(ax,ay,px,py) > (w + del) && dis(bx,by,px,py) > (w + del)) return false;
    if(! ((dis(0,0,ax,ay) < (w - del) && dis(px,py,ax,ay) < (w - del)) || (dis(0,0,bx,by) < (w - del) && dis(px,py,bx,by) < (w - del)))){//起点和目的地不在同一个圆
        if(dis(ax,ay,bx,by) > (2 * w + del)) return false;
    }
    return true;
}

double find(double l,double r){
    double mid = (l + r) / 2;
    double ans = 0.0;

    while(l <= (r - del)){
        if(p(mid = (l + r) / 2)){
            ans = mid;
            r = mid;
        }
        else l = mid;
    }

    return ans;
}

int main(){
    ll t = 0; cin>>t;
    double ans;
    while(t--){
        cin>>px>>py>>ax>>ay>>bx>>by;
        double l = 0.1,r = 1e6;
        ans = find(l,r);
        printf("%.10lf\n",ans);
    }
    
    return 0;
}