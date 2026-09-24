// 「字节值」怎么算 —— 公式推导 + 穷举验证
#include <bits/stdc++.h>
using namespace std;

int main() {
    printf("===== ① 四种算法对照（重点看负数那一栏）=====\n");
    printf("  %-9s %-10s %-16s %-10s %s\n", "v", "v & 0xFF", "(unsigned char)v", "v % 256", "v-256*floor(v/256)");
    int vs[] = {0, 1, 127, 128, 255, 256, 257, 1000, 1002, 65536, -1, -2, -256, -257, -1002};
    for (int v : vs)
        printf("  %-9d %-10d %-16d %-10d %d\n", v, v & 0xFF, (unsigned char)v, v % 256,
               v - 256 * (int)floor(v / 256.0));
    printf("  ↑ v %% 256 在负数上【错】（C++ 的 %% 保留符号：-1 %% 256 = -1，不是 255）\n");

    printf("\n===== ② 二进制视角：字节值 = 只留低 8 位 =====\n");
    for (int v : {1002, 1000, 255, 256, -1}) {
        int b = v & 0xFF, sum = 0;
        for (int i = 0; i < 8; ++i) if ((b >> i) & 1) sum += (1 << i);
        printf("  v=%-6d 低 8 位 = ", v);
        for (int i = 7; i >= 0; --i) printf("%d", (b >> i) & 1);
        printf(" = %d = 0x%02X   （按 128/64/32/16/8/4/2/1 加权求和 = %d）\n", b, b, sum);
    }

    printf("\n===== ③ 十进制怎么手算：就是「减去整数个 256」=====\n");
    for (int v : {1002, 1000, 70000, 300}) {
        int q = (int)floor(v / 256.0), b = v - 256 * q;
        printf("  %d = 256 × %d + %d   ⇒ 字节值 = %d = 0x%02X\n", v, q, b, b, b);
    }
    printf("  1002 = 256×3 + 234   （256×3=768，1002−768=234=0xEA）\n");

    printf("\n===== ④ 反过来：字节值 b 会被填成什么数（还原公式）=====\n");
    printf("  int(4 字节)        = 0x01010101 × b\n");
    printf("  long long(8 字节)  = 0x0101010101010101 × b\n");
    for (int b : {0, 1, 0x3f, 0x7f, 0xea, 0xff}) {
        int ai[1]; long long al[1];
        memset(ai, b, sizeof(ai));
        memset(al, b, sizeof(al));
        int fi = (int)(0x01010101u * (unsigned)b);
        long long fl = (long long)(0x0101010101010101ULL * (unsigned long long)b);
        printf("  b=0x%02X  int 实测=%-12d 公式=%-12d %s ; ll 实测=%-21lld 公式=%-21lld %s\n",
               b, ai[0], fi, ai[0] == fi ? "✔" : "✘", al[0], fl, al[0] == fl ? "✔" : "✘");
    }

    printf("\n===== ⑤ 穷举验证：256 个字节值 × 2 种宽度，公式全对？=====\n");
    int bad = 0;
    for (int b = 0; b < 256; ++b) {
        int ai[1]; long long al[1];
        memset(ai, b, sizeof(ai));
        memset(al, b, sizeof(al));
        if (ai[0] != (int)(0x01010101u * (unsigned)b)) ++bad;
        if (al[0] != (long long)(0x0101010101010101ULL * (unsigned long long)b)) ++bad;
    }
    printf("  512 组核对：错 %d 组 %s\n", bad, bad == 0 ? "✔ 还原公式成立" : "✘");

    printf("\n===== ⑥ 穷举验证：任意 v 与 v&0xFF 填出来的结果相同？=====\n");
    bad = 0;
    for (int v = -200000; v <= 200000; ++v) {
        int a1[1], a2[1];
        memset(a1, v, sizeof(a1));
        memset(a2, v & 0xFF, sizeof(a2));
        if (a1[0] != a2[0]) ++bad;
    }
    printf("  v ∈ [-200000, 200000] 共 400001 个：不一致 %d 组 %s\n", bad, bad == 0 ? "✔" : "✘");
    return 0;
}
