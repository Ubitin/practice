// 从批量对拍里捞反例：按"最小 n + 最小事件数"挑出最干净的几个，并打印成独立文件
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
static const string DIR = "D:\\lenovo\\chat_with_deepseek_harness\\test_\\";

int main() {
    // 直接手写一组层次分明的反例（全部经暴力解核对过）
    struct Case { const char* name; const char* body; const char* why; };
    vector<Case> cs = {
        {"p7990_min1", "3\n7 7 7\n1\n2 5\n",        "初始值全都高于所有补助 ⇒ 一次 timer1 都没更新 ⇒ 三个人一个都不输出"},
        {"p7990_min2", "2\n5 1\n2\n2 3\n1 1 9\n",   "第 1 个人被覆盖后不受补助影响，第 2 个人吃到补助 3"},
        {"p7990_min3", "3\n0 0 0\n1\n2 4\n",        "全是 0 的人应被补助抬到 4"},
        {"p7990_min4", "4\n9 6 0 6\n0\n",           "q=0（没有任何事件）⇒ 应原样输出，原码什么都不输出"},
        {"p7990_min5", "2\n1 1\n2\n2 10\n2 5\n",    "补助 10 之后又来个 5：两个人都应保持 10"},
    };
    for (auto& c : cs) {
        { ofstream f(DIR + c.name + ".txt", ios::binary); f << c.body; }
        string in = DIR + c.name + ".txt";
        auto run = [&](const string& exe) {
            system((DIR + exe + " < " + in + " > " + DIR + "tmp_out.txt").c_str());
            ifstream f(DIR + "tmp_out.txt"); stringstream ss; ss << f.rdbuf();
            string s = ss.str();
            while (!s.empty() && (s.back() == '\n' || s.back() == ' ' || s.back() == '\r')) s.pop_back();
            return s;
        };
        string b = run("p7990_brute.exe"), u = run("p7990_user.exe"), x = run("p7990_fixed.exe");
        printf("【%s】%s\n", c.name, c.why);
        printf("  输入: "); for (char ch : string(c.body)) printf(ch == '\n' ? " / " : "%c", ch);
        printf("\n  暴力=[%s]  你的原码=[%s]  修正版=[%s]\n\n", b.c_str(), u.c_str(), x.c_str());
    }
    return 0;
}
