#include <bits/stdc++.h>
using namespace std;

int main() {
    string s = "Hello World";     // size = 11，下标 0~10

    // ===== 1. 两种基本用法 =====
    cout << "[" << s.substr(6) << "]\n";        // [World]     一个参数：从下标 6 到结尾
    cout << "[" << s.substr(0, 5) << "]\n";     // [Hello]     两个参数：从 0 开始取 5 个
    cout << "[" << s.substr(6, 5) << "]\n";     // [World]     从 6 取 5 个
    cout << "[" << s.substr(3, 100) << "]\n";   // [lo World]  count 超过结尾：自动截到结尾（不报错）

    // ===== 2. 参数越界的行为 =====
    cout << "[" << s.substr(11) << "]\n";       // []  pos == size()：合法，返回空串
    try {
        s.substr(100);                          // pos > size()：抛 out_of_range 异常
    } catch (const out_of_range& e) {
        cout << "pos越界抛异常: " << e.what() << "\n";
    }

    // ===== 3. 常见应用场景 =====
    // ① 取文件名后缀
    string file = "photo.jpg";
    size_t dot = file.find('.');
    cout << "后缀: " << file.substr(dot + 1) << "\n";   // jpg

    // ② 分割字符串（P1553 拆数就是这套）！
    string num = "600.084";
    size_t p = num.find('.');
    string a = num.substr(0, p);       // "600"
    string b = num.substr(p + 1);      // "084"
    cout << "整数部分=" << a << " 小数部分=" << b << "\n";

    // ③ 取定长字段（学号/身份证/日期）
    string id = "2024090101";
    cout << "年=" << id.substr(0, 4) << " 月=" << id.substr(4, 2) << "\n";

    // ===== 4. 空串与边界 =====
    string empty = "";
    cout << "空串 substr(0) = [" << empty.substr(0) << "]\n";

    // ===== 5. 中文注意：substr 按字节切（UTF-8 一个汉字 3 字节） =====
    string zh = "你好世界";
    cout << "zh.size() = " << zh.size() << " （12 字节，不是 4）\n";
    cout << "zh.substr(0,3) = [" << zh.substr(0, 3) << "]\n";   // 恰好一个汉字"你"
    return 0;
}
