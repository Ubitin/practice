#include<bits/stdc++.h>
using namespace std;

string add(string a, string b){
    reverse(a.begin(), a.end());  reverse(b.begin(), b.end());   // 个位在前好进位
    size_t len = max(a.size(), b.size());
    string ans(len, '0');  int carry = 0;
    for (size_t i = 0; i < len; ++i) {
        int da = i < a.size() ? a[i] - '0' : 0;   // 短的越界补 0（三目，绝不真越界）
        int db = i < b.size() ? b[i] - '0' : 0;
        int t = da + db + carry;
        ans[i] = char('0' + t % 10);
        carry = t / 10;
    }
    if (carry) ans.push_back('1');
    reverse(ans.begin(), ans.end());
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m = 0,n = 0; cin>>m>>n;

    int d = n - m + 1;

    vector<string>f; f.resize(1001);

    f[1] = "1"; f[2] = "1";

    if(d == 2) cout<<"1";
    else if(d >= 3){
        for(int i = 3;i<=d;++i){
            f[i] = add(f[i - 1],f[i - 2]);
        }

        cout<<f[d];
    }

    return 0;
}