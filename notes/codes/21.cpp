#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===== 1. cin >> 遇空白停（只能读"一个词"）=====
    string w1, w2;
    cin >> w1 >> w2;                    // 输入: hello world
    cout << "cin >> 读两个词: [" << w1 << "] [" << w2 << "]\n";
    cin.ignore();                       // 清掉行尾残留的换行

    // ===== 2. getline 读整行（含空格）=====
    string line;
    getline(cin, line);                 // 输入: hello world again
    cout << "getline 读整行: [" << line << "]\n";

    // ===== 3. 经典坑：cin 读数字后直接 getline 会读到空串 =====
    int n;
    cout << "输入一个数字: ";
    cin >> n;
    getline(cin, line);                 // ❌ 读到的是残留的 '\n' → 空串
    cout << "没 ignore 直接 getline: [" << line << "]\n";
    getline(cin, line);                 // ✅ 再读才是真正内容
    cout << "修复后 getline: [" << line << "]\n";

    // ===== 4. getline 第三参数：自定义分隔符 =====
    string part;
    getline(cin, part, ',');            // 输入: apple,banana
    cout << "按逗号切第一段: [" << part << "]\n";
    getline(cin, part);                 // 读剩下的 banana
    cout << "剩下: [" << part << "]\n";

    // ===== 5. while(getline) 连续读多行（读到 EOF 结束）=====
    string s;
    int cnt = 0;
    while (getline(cin, s)) {           // 输入两行后按 Ctrl+Z(Windows)/Ctrl+D
        cnt++;
        cout << "第" << cnt << "行: [" << s << "]\n";
        if (cnt >= 3) break;            // 演示用，避免死等
    }
    return 0;
}
