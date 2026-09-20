#include <bits/stdc++.h>
using namespace std;

// ============ 候选实现：test_\01_isbn_fixed2.cpp 的逻辑（原样搬入） ============
static bool candOK(const string& s, string& out){
    int hy = (int)count(s.begin(), s.end(), '-');
    if(hy > 3) return false;
    if(s.empty() || s.front() == '-' || s.back() == '-') return false;
    for(size_t i = 1; i < s.size(); ++i)
        if(s[i] == '-' && s[i-1] == '-') return false;
    if(hy == 3 && s[s.size() - 2] != '-') return false;

    string d; int S = 0, n = (int)s.size(), cnt = 0;
    for(int i = 0; i < n; ++i){
        char c = s[i];
        if(c == '-') continue;
        int v;
        if(c == 'X'){ if(i != n-1) return false; v = 10; }
        else if(isdigit((unsigned char)c)) v = c - '0';
        else return false;
        ++cnt; S += (11 - cnt) * v; d += c;
    }
    if(cnt != 10 || S % 11) return false;

    string f12 = "978" + d.substr(0, 9);
    int s13 = 0;
    for(int i = 0; i < 12; ++i) s13 += ((i % 2 == 0) ? 1 : 3) * (f12[i] - '0');
    out = "978-" + s.substr(0, s.size() - 1) + char('0' + (10 - s13 % 10) % 10);
    return true;
}

// ============ 判官：按题面文字独立推导（连字符按"插入位置枚举"判定） ============
// 规则：① 连字符 ≤3 ② 不在首尾 ③ 不连续 ④ 若恰有 3 个，则必须有一个紧挨校验码之前
//       数字（X 仅末位、值 10）恰 10 个；加权和 ≡0 (mod 11)；否则 invalid
static bool refOK(const string& s, string& out){
    int n = (int)s.size();
    // ① 连字符位置集合
    vector<int> hp;
    for(int i = 0; i < n; ++i) if(s[i] == '-') hp.push_back(i);
    if((int)hp.size() > 3) return false;
    if(!hp.empty() && (hp.front() == 0 || hp.back() == n-1)) return false;      // ②
    for(size_t i = 1; i < hp.size(); ++i)
        if(hp[i] == hp[i-1] + 1) return false;                                  // ③
    if(hp.size() == 3 && hp.back() != n-2) return false;                        // ④ 校验码前必须断开
    // 数字展开
    string d; int S = 0, k = 0;
    for(int i = 0; i < n; ++i){
        char c = s[i];
        if(c == '-') continue;
        if(c == 'X'){
            if(i != n-1) return false;          // X 只能是校验位
            ++k; S += (11 - k) * 10; d += 'X';
        } else if('0' <= c && c <= '9'){
            ++k; S += (11 - k) * (c - '0'); d += c;
        } else return false;
    }
    if(k != 10 || S % 11 != 0) return false;
    // 输出：978- + 去掉旧校验位 + 新校验位
    int s13 = 0;
    string f12 = "978" + d.substr(0, 9);
    for(int i = 0; i < 12; ++i) s13 += (i % 2 == 0 ? 1 : 3) * (f12[i] - '0');
    out = "978-" + s.substr(0, n-1) + char('0' + (10 - s13 % 10) % 10);
    return true;
}

int main(){
    // 生成 10 位数字（部分校验和合法、部分非法），枚举所有 ≤3 个连字符的插入模式
    mt19937 rng(20260912);
    long long cases = 0, bad = 0;
    vector<string> seeds;
    // 手工种子：题面合法/非法示例
    seeds = {"0394280132","0394280131","3540425802","0140283333","0306406152","0804429571","9000000002"};

    vector<vector<int>> patterns;                       // 连字符插入位置（在字符之间的 9 个空隙 1..9）
    for(int a = 1; a <= 9; ++a){
        patterns.push_back({a});                        // 1 个连字符
        for(int b = 1; b <= 9; ++b){
            patterns.push_back({a,b});                  // 2 个
            for(int c = 1; c <= 9; ++c) patterns.push_back({a,b,c});   // 3 个（含连续，考非法情形）
        }
    }
    patterns.push_back({});                             // 0 个连字符

    for(int iter = 0; iter < 300; ++iter){
        string d;
        if(iter < (int)seeds.size()) d = seeds[iter];
        else { d.resize(10); for(char& c : d) c = char('0' + rng() % 10); }
        for(int xpos = -1; xpos < 10; ++xpos){          // -1：无 X；否则把该位换成 X
            string base = d;
            if(xpos >= 0) base[xpos] = 'X';
            for(auto& pat : patterns){
                // 把连字符插到基础串的空隙处，按位置从大到小插入保证下标正确
                string s = base;
                vector<int> p = pat;
                sort(p.rbegin(), p.rend());
                for(int pos : p) s.insert(s.begin() + pos, '-');
                string o1, o2;
                bool r1 = candOK(s, o1), r2 = refOK(s, o2);
                ++cases;
                if(r1 != r2 || (r1 && o1 != o2)){
                    ++bad;
                    if(bad <= 5) cout << "MISMATCH: [" << s << "] cand=" << (r1?o1:"invalid") << " ref=" << (r2?o2:"invalid") << "\n";
                }
            }
        }
    }
    cout << "cases=" << cases << "  bad=" << bad << "\n";
    return 0;
}
