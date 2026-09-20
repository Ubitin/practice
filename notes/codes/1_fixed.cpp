#include<bits/stdc++.h>
using namespace std;
int main(){
    string s = "hello world";
    copy(s.begin(), s.end(), ostream_iterator<char>(cout));       // 正确类型 char
    cout << "\n";
    copy(s.begin(), s.end(), ostream_iterator<char>(cout, " "));  // 带分隔符
    return 0;
}
