#include<bits/stdc++.h>
using namespace std;
string rev(string s){
    reverse(s.begin(),s.end());
    size_t i = 0;
    while(i<s.size()&&s[i]=='0') ++i;
    if(s == "0") return "0";
    else return s.substr(i);
}

int main(){
    string s;
    cin>>s;
    if(s.find('%') != string::npos){
        cout<<rev(s.erase(s.size()-1))<<'%';
    }
    else if(s.find('/') != string::npos){
        string t1,t2;
        t1 = s.substr(0,s.find('/')); t2 = s.substr(s.find('/') + 1);
        t1 = rev(t1); t2 = rev(t2);
        cout<<t1<<'/'<<t2;
    }
    else if(s.find('.') != string::npos){
        string v1,v2;
        v1 = s.substr(0,s.find('.'));
        v2 = s.substr(s.find('.') + 1); 
        size_t j = 0;
        while(j<v2.size()&&v2[j]=='0') ++j;
        if(v2=="0") v2 = "0";
        else v2 = v2.substr(j);
        v1 = rev(v1); v2 = rev(v2);
        cout<<v1<<'.'<<v2;
    }
    else{
        s = rev(s);
        cout<<s;
    }
    return 0;    
}