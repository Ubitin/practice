#include <bits/stdc++.h>
using namespace std;

long long b[2][2];
int c[2][2];

int main() {
    memset(b, 1002, sizeof(b));   // 想给每个 long long 元素赋 1002
    memset(c, 1002, sizeof(c));   // 想给每个 int 元素赋 1002

    printf("long long b[0][0] = %lld\n", b[0][0]);
    printf("  b[0][0] == 1002 ? %s\n", b[0][0] == 1002 ? "是" : "否");
    printf("  b[0][0] > 0      ? %s\n", b[0][0] > 0 ? "是" : "否");
    printf("  min(5, b[0][0])  = %lld  <- 我想记录陨石时刻 5\n", min(5LL, b[0][0]));
    printf("  (0+1) >= b[0][1] ? %s  <- 会被当成“已经炸了”\n", (0 + 1) >= b[0][1] ? "是" : "否");

    printf("int       c[0][0] = %d\n", c[0][0]);
    printf("  c[0][0] == 1002 ? %s\n", c[0][0] == 1002 ? "是" : "否");

    int d[2][2];
    memset(d, -1, sizeof(d));     // -1 的每个字节都是 0xFF，memset 才正确
    printf("memset(d,-1) -> d[0][0] = %d  (正确)\n", d[0][0]);
    return 0;
}
