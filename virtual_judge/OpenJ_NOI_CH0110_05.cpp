#include<bits/stdc++.h>
using namespace std;

bool cmp(pair<int,int>s1,pair<int,int>s2){
    if(s1.first != s2.first) return s1.first > s2.first;
    else if(s1.first == s2.first) return s1.second < s2.second;
}

int main(){
    size_t n = 0,m= 0;
    cin>>n>>m;

    int bound = m * 150 / 100;//欲录取面试的人数

    vector<pair<int,int>>s;//参加考试的人的数组
    s.resize(n);

    for(size_t i = 0;i<n;++i) cin>>s[i].second>>s[i].first;//录入报名号和笔试成绩

    sort(s.begin(),s.end(),cmp);

    int score = s[bound - 1].first;

    size_t j = 0;
    for(;s[j].first>=score;j++);
    size_t res = j;
    
    cout<<score<<" "<<res<<"\n";

    for(size_t i = 0;s[i].first >= score;i++) cout<<s[i].second<<" "<<s[i].first<<"\n";

    return 0;
}