// P1160 —— 你的代码 + 补上两行（其余结构完全保留你的写法：构造函数参数顺序 key,pre,nxt）
#include<bits/stdc++.h>
using namespace std;
using ll = long long;

const ll maxn = 1e5 + 2;
struct node{
    ll pre,nxt,key;
    node( ll _key = 0 , ll _pre = 0 , ll _nxt = 0){
        key = _key; pre = _pre; nxt = _nxt;
    };
};

ll n,m; ll indexx[maxn]; ll tot = 0;
node a[maxn];

void ins_back(ll x,ll y){                 // 把 y 插到 x 的右边
    ll now = indexx[x];
    a[++tot] = {y,now,a[now].nxt};
    a[a[now].nxt].pre = tot;
    a[now].nxt = tot;                     // ★ 补上这一行
    indexx[y] = tot;
}

void ins_front(ll x,ll y){                // 把 y 插到 x 的左边
    ll now = indexx[x];
    a[++tot] = {y,a[now].pre,now};
    a[a[now].pre].nxt = tot;
    a[now].pre = tot;                     // ★ 补上这一行
    indexx[y] = tot;
}

void del(ll x){
    ll now = indexx[x];
    ll le = a[now].pre , rt = a[now].nxt;
    a[le].nxt = rt;
    a[rt].pre = le;
    indexx[x] = 0;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    a[0] = node();
    ins_back(0,1);
    cin>>n;
    for(ll i = 2;i<=n;++i){
        ll k,p; cin>>k>>p;
        p ? ins_back(k,i) : ins_front(k,i);
    }

    cin>>m;
    while(m--){
        ll temp; cin>>temp;
        if(indexx[temp]) del(temp);
    }

    ll now = a[0].nxt;
    while(now){
        cout<<a[now].key<<" ";
        now = a[now].nxt;
    }

    return 0;
}
