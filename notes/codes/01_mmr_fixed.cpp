#include<bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) cin >> v[i];

    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());   // 去重三连：标准解法

    cout << v.size() << "\n";
    for (int x : v) cout << x << " ";
    cout << "\n";
    return 0;
}
