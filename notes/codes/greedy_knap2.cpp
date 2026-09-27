#include <bits/stdc++.h>
using namespace std;
int main(){
    struct T{int C; vector<int> w,v;};
    vector<T> ts = {
        {5, {4,3,3}, {5,4,4}},     // 性价比 4/3≈1.33 > 5/4=1.25 ⇒ 贪心先拿 3
        {6, {6,4,4}, {6,4,4}},     // 性价比 6/6=1 > 4/4=1（相等，看排序）⇒ 危险
        {7, {7,5,5}, {7,5,6}},     // 7/7=1 vs 6/5=1.2 ⇒ 贪心先拿 (5,6)
        {8, {5,4,4}, {6,4,4}},     // 6/5=1.2 最高 ⇒ 贪心先拿 5
    };
    for (auto&t:ts){
        int n=t.w.size();
        vector<int> dp(t.C+1,0);
        for(int i=0;i<n;++i) for(int c=t.C;c>=t.w[i];--c) dp[c]=max(dp[c],dp[c-t.w[i]]+t.v[i]);
        vector<int> id(n); iota(id.begin(),id.end(),0);
        sort(id.begin(),id.end(),[&](int a,int b){ return (double)t.v[a]/t.w[a] > (double)t.v[b]/t.w[b]; });
        int C=t.C, g=0;
        for(int k=0;k<n;++k){int i=id[k]; if(t.w[i]<=C){C-=t.w[i]; g+=t.v[i];}}
        printf("容量 %d 物品", t.C);
        for(int i=0;i<n;++i) printf(" (w=%d,v=%d)", t.w[i], t.v[i]);
        printf("  贪心=%d  最优=%d  %s\n", g, dp[t.C], g==dp[t.C] ? "（贪心也对）" : "✘ 贪心错");
    }
    return 0;
}
