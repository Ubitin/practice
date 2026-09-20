#include<bits/stdc++.h>
using namespace std;
int main(){
    vector<int>v={1,2,3,4,5};
    for(auto it = v.begin();it != v.end();++it) cout<<*it<<" ";
    for(int x:v) cout<<x<<" ";

    // ① 下标循环（最经典！和数组写法完全一样，你居然漏了这个）
    for (int i = 0; i < v.size(); i++) cout << v[i] << " ";

    // ② for_each + lambda（STL 算法库，OI Wiki 算法章的思路）
    for_each(v.begin(), v.end(), [](int x){ cout << x << " "; });

    // ③ copy + ostream_iterator（STL 全家桶一行流式输出）
    copy(v.begin(), v.end(), ostream_iterator<int>(cout, " "));

    // ④ 反向迭代器（倒序打印）
    for (auto it = v.rbegin(); it != v.rend(); ++it) cout << *it << " ";

    // ⑤ while + 迭代器（指针风格的紧凑写法）
    auto it = v.begin();
    while (it != v.end()) cout << *it++ << " ";

    //一个实用细节（课程/考试可能考）：v.size() 返回的是无符号 size_t，int i 和它比较会有类型警告，严谨写法是：
    for (size_t i = 0; i < v.size(); i++);
    // 或者：for (int i = 0; i < (int)v.size(); i++) ...   // 强转，竞赛常用
    return 0;
}