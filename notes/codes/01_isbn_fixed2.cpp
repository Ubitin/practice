#include<bits/stdc++.h>
using namespace std;


static bool okStruct(const string& s){
    int hy = (int)count(s.begin(), s.end(), '-');
    if(hy > 3) return false;                                  
    if(s.front() == '-' || s.back() == '-') return false;     
    for(size_t i = 1; i < s.size(); ++i)
        if(s[i] == '-' && s[i-1] == '-') return false;       
    if(hy == 3 && s[s.size() - 2] != '-') return false;       
    return true;
}


static bool valid10(const string& s, string& d){
    d.clear();
    int S = 0;
    int n = (int)s.size();
    int digitCnt = 0;
    for(int i = 0; i < n; ++i){
        char c = s[i];
        if(c == '-') continue;
        int v;
        if(c == 'X'){                       
            if(i != n - 1) return false;
            v = 10;
        } else if(isdigit((unsigned char)c)){
            v = c - '0';
        } else return false;                
        ++digitCnt;
        S += (11 - digitCnt) * v;           
        d += c;
    }
    if(digitCnt != 10) return false;        
    return S % 11 == 0;
}


static char check13(const string& first12){
    int S = 0;
    for(int i = 0; i < 12; ++i) S += ((i % 2 == 0) ? 1 : 3) * (first12[i] - '0');
    return char('0' + (10 - S % 10) % 10);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T; cin >> T;
    while(T--){
        string s; cin >> s;
        string d;
        if(!okStruct(s) || !valid10(s, d)){
            cout << "invalid\n";
            continue;
        }
        char cd = check13("978" + d.substr(0, 9));
        cout << "978-" << s.substr(0, s.size() - 1) << cd << "\n";
    }
    return 0;
}
