#include<bits/stdc++.h>
using namespace std;

int main(){
    double r,g,b;
    cin >> r >> g >> b;
    r /= 255.0; g /= 255.0; b /= 255.0;      
    double M = max(max(r,g), max(g,b));
    double m = min(min(r,g), min(g,b));
    double d = M - m;                        
    double V = M;
    double S = (M == 0.0) ? 0.0 : d / M;    
    double H = 0.0;                          
    if (d != 0.0) {                          
        if (M == r)      H = 60.0 * (0.0 + (g - b) / d);
        else if (M == g) H = 60.0 * (2.0 + (b - r) / d);
        else             H = 60.0 * (4.0 + (r - g) / d);
        if (H < 0.0) H += 360.0;             
    }
    printf("%.4f,%.4f%%,%.4f%%\n", H, S*100.0, V*100.0);   // 百分号写 %%

    return 0;
}