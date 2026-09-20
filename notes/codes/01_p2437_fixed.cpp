#include<bits/stdc++.h>
using namespace std;

// P2437 蜜蜂路线（修正版，独立新文件，不动你的 .\test_\01.cpp）
// 模型：从 m 爬到 n，每次只能到标号更大 1 或 2 的相邻蜂房 → 方案数 = F(n-m+1)，F(1)=F(2)=1
// 关键：n ≤ 1000，答案可达 200+ 位十进制数字 → long long 装不下（d ≥ 93 就溢出），必须高精度

// 高精度加法（字符串形式，低位在前处理进位）
string add(const string& a, const string& b){
    string x = a, y = b;
    reverse(x.begin(), x.end());
    reverse(y.begin(), y.end());
    string r;
    int carry = 0;
    for(size_t i = 0; i < x.size() || i < y.size() || carry; ++i){
        int da = (i < x.size()) ? x[i] - '0' : 0;
        int db = (i < y.size()) ? y[i] - '0' : 0;
        int t = da + db + carry;
        r += char('0' + t % 10);
        carry = t / 10;
    }
    reverse(r.begin(), r.end());
    return r;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n; cin >> m >> n;
    int d = n - m + 1;                 // 只与距离有关：答案是 F(d)

    vector<string> f(d + 1);
    f[1] = "1";                        // 边界：F(1) = F(2) = 1
    if(d >= 2) f[2] = "1";
    for(int i = 3; i <= d; ++i)
        f[i] = add(f[i - 1], f[i - 2]); // 递推 + 高精度

    cout << f[d] << "\n";
    return 0;
}
