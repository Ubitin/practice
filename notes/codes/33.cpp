// 高精度去前导零 —— 演示程序（配套讲义：高精度去前导零详解.md）
// 编译: g++ -std=c++17 -O2 -o 33.exe 33.cpp
// 注意: 本机 g++ 按 GBK 解码源文件，字符串字面量里不要放中文（注释可以）
#include <bits/stdc++.h>
using namespace std;

// ① 惯用写法：find_first_not_of + 全零兜底
string trimA(const string& s) {
    size_t i = s.find_first_not_of('0');
    if (i == string::npos) return "0";        // 全是 0（或空串）-> 结果必须是 "0"
    return s.substr(i);
}
// ② 手写循环：保留至少一位，天然不需要兜底
string trimB(string s) {
    size_t i = 0;
    while (i + 1 < s.size() && s[i] == '0') ++i;
    return s.substr(i);
}
// ③ 最简惯性写法（危险）：不加兜底
string trimRaw(const string& s) {
    string t = s;
    t.erase(0, t.find_first_not_of('0'));
    return t;
}

string bigSub(const string& A, const string& B, bool trim, bool keepOne) {
    vector<int> a, b, c;
    for (char ch : A) a.push_back(ch - '0');
    for (char ch : B) b.push_back(ch - '0');
    reverse(a.begin(), a.end()); reverse(b.begin(), b.end());   // 低位在前
    int borrow = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int t = a[i] - (i < b.size() ? b[i] : 0) - borrow;
        if (t < 0) { t += 10; borrow = 1; } else borrow = 0;
        c.push_back(t);
    }
    if (trim) {                                  // 高位在 c 的尾部 -> 从尾部删
        if (keepOne) while (c.size() > 1 && c.back() == 0) c.pop_back();
        else         while (!c.empty() && c.back() == 0) c.pop_back();
    }
    string r;
    for (int i = int(c.size()) - 1; i >= 0; --i) r += char('0' + c[i]);
    return r;
}

int main() {
    cout << "=== (1) three ways on dirty inputs ===\n";
    cout << "input        trimA(find+fallback)  trimB(manual loop)  trimRaw(no fallback)\n";
    for (string s : {string("000123"), string("0"), string("0000"), string("100"),
                     string("0100"), string("001000"), string(""), string("10")}) {
        string a = trimA(s), b = trimB(s), c = trimRaw(s);
        printf("%-12s %-22s %-19s %s\n", ("\"" + s + "\"").c_str(),
               ("\"" + a + "\"").c_str(), ("\"" + b + "\"").c_str(),
               c.empty() ? "\"\"  <- EMPTY, bug!" : ("\"" + c + "\"").c_str());
    }
    cout << "note: trimRaw(\"0000\") returns an empty string, not \"0\" -> must add fallback\n";

    cout << "\n=== (2) find_first_not_of returns npos on all-zero ===\n";
    {
        string z = "0000";
        cout << "s = \"0000\": find_first_not_of('0') == npos ? "
             << (z.find_first_not_of('0') == string::npos) << "   (this is the trap)\n";
        string e = z; e.erase(0, e.find_first_not_of('0'));
        cout << "erase(0, npos) empties it: size = " << e.size() << "\n";
    }

    cout << "\n=== (3) high-precision subtract: leading zeros appear at the HIGH end ===\n";
    for (auto [A, B] : vector<pair<string, string>>{{"1000", "999"}, {"500", "500"},
                                                    {"123456", "123455"}, {"100", "1"}}) {
        cout << A << " - " << B << ":  raw = \"" << bigSub(A, B, false, true)
             << "\"   trimmed = \"" << bigSub(A, B, true, true)
             << "\"   trimmed-without-guard = \"" << bigSub(A, B, true, false) << "\"\n";
    }
    cout << "note: low-digit-first storage keeps the leading zeros at c.back();\n"
            "      the size()>1 guard is what keeps a lone 0 alive.\n";

    cout << "\n=== (4) leading zeros only exist at the MOST significant end ===\n";
    {
        string s = "00123400";                       // 低位端也有 0，但那些是有效数字
        cout << "s = \"" << s << "\"  ->  trimA = \"" << trimA(s) << "\"\n";
        string wrong = s;                            // 错误示范：从最低位那端删
        while (wrong.size() > 1 && wrong.back() == '0') wrong.pop_back();
        cout << "wrong end (pop_back): \"" << wrong << "\"  <- 把有效位当成了前导零\n";
    }

    cout << "\n=== (5) digit-reversal style (P1553): reverse then strip, with 0 fallback ===\n";
    auto revInt = [](string t) {                     // 反转 + 去前导零 + 全零兜底
        reverse(t.begin(), t.end());
        size_t i = t.find_first_not_of('0');
        if (i == string::npos) return string("0");
        return t.substr(i);
    };
    for (string s : {string("600"), string("0"), string("000"), string("1200"), string("100000")})
        cout << "\"" << s << "\" -> rev -> \"" << revInt(s) << "\"\n";

    cout << "\n=== (6) do NOT trim when the task needs a fixed width ===\n";
    {
        string s = "007";                            // 例如"补零到 3 位"的编号
        cout << "s = \"" << s << "\"  trimA = \"" << trimA(s)
             << "\"  <- 若题目要 3 位编号，去掉前导零反而错\n";
    }
    return 0;
}
