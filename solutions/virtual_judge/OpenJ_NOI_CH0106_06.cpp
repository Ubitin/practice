#include<bits/stdc++.h>
using namespace std;


int main(){

    int l,m;
    cin>>l>>m;

    vector<pair<int,int>>tree;
    tree.resize(m);

    for(int i = 0;i<m;++i){
        cin>>tree[i].first>>tree[i].second;
    }

    sort(tree.begin(),tree.end());

    int curL = tree[0].first; int curR = tree[0].second;

    int t = 0;
    
    for(int i =1;i<m;++i){

        if(tree[i].first<=curR){
            curR = max(tree[i].second,curR);
        }
        else{
            t += (curR - curL + 1);
            curL = tree[i].first; curR = tree[i].second;
        }

    }

    t += curR - curL + 1;

    cout<<l - t + 1;

    return 0;

}