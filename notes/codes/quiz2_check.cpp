#include <bits/stdc++.h>
using namespace std;
int main() {
    vector<int> v = {1, 2, 3};
    // ① 无分隔符
    copy(v.begin(), v.end(), ostream_iterator<int>(cout));      // 期望 123
    cout << "\n";
    // ② 带分隔符
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " ")); // 期望 1 2 3 （末尾也有空格）
    cout << "\n";
    // ③ 逗号分隔
    vector<int> w = {5, 4, 3};
    copy(w.begin(), w.end(), ostream_iterator<int>(cout, ", "));// 期望 5, 4, 3,
    cout << "\n";
    // ④ int → double 隐式转换（能编译！int 可隐式转 double）
    copy(v.begin(), v.end(), ostream_iterator<double>(cout, " ")); // 期望 1 2 3
    cout << "\n";
    // ⑤ char → int：打印 ASCII 码值
    vector<char> ch = {'A', 'B', 'C'};
    copy(ch.begin(), ch.end(), ostream_iterator<int>(cout, " "));  // 期望 65 66 67
    cout << "\n";
    // ⑥ vector<string> 才配 ostream_iterator<string>
    vector<string> names = {"a", "bb"};
    copy(names.begin(), names.end(), ostream_iterator<string>(cout, "|")); // 期望 a|bb|
    cout << "\n";
    return 0;
}
