// 反面教材：如果不用 __int128，同样的式子会怎么错
// 用来验证"这题到底需不需要 __int128"这件事——答案是【必须】，但【不需要高精度】
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
using ull128 = unsigned __int128;

static void printU128(ull128 v) {
    if (v == 0) { putchar('0'); return; }
    char buf[48]; int p = 0;
    while (v > 0) { buf[p++] = char('0' + (int)(v % 10)); v /= 10; }
    while (p) putchar(buf[--p]);
}

int main() {
    ull a, b, m;
    cin >> a >> b >> m;

    // ① 直接 long long 相乘（错）
    long long bad1 = (long long)a * (long long)b % (long long)m;
    // ② 升到 unsigned long long 也不行（1.84e19 < 1e36）
    ull bad2 = a * b % m;
    // ③ 先升到 __int128 再乘（对）
    ull128 good = (ull128)a * b % m;

    printf("① long long 直乘      : %lld\n", bad1);
    printf("② unsigned long long   : %llu\n", bad2);
    printf("③ __int128            : "); printU128(good); putchar('\n');
    return 0;
}
