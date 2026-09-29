// 探针：逐字符打印，看 temp 里到底是什么（复刻用户逻辑 + 打点）
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main() {
    string num;
    if (!(cin >> num)) { printf("读入失败\n"); return 0; }
    printf("读到的输入 = [%s]，长度 = %zu\n", num.c_str(), num.size());

    for (ll i = 0; i < (ll)num.size(); ++i) {
        char c = num[i];
        string temp;                       // ★ 用户的写法：每轮都新建（清空）
        if (c >= '0' && c <= '9') { temp += c; printf("  i=%lld c='%c' 数字: temp=[%s]\n", i, c, temp.c_str()); }
        else if (c == '.') {
            printf("  i=%lld c='.' → 准备 stoll(temp)，但此时 temp=[%s] 长度=%zu\n", i, temp.c_str(), temp.size());
            if (temp.empty()) { printf("  ✘✘ temp 是空串！stoll(空串) 会抛 std::invalid_argument\n"); return 1; }
            ll v = stoll(temp);
            printf("      → 成功读入 %lld\n", v);
        } else {
            printf("  i=%lld c='%c' 运算符\n", i, c);
        }
    }
    return 0;
}
