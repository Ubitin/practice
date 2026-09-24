// 「字节值」到底是什么 —— 全部用代码打出来看
#include <bits/stdc++.h>
using namespace std;

template <class T> void dumpBytes(const char* name, T v) {
    unsigned char b[sizeof(T)];
    memcpy(b, &v, sizeof(T));
    printf("  %-18s = %-22lld  内存里的字节(从低地址到高地址): ", name, (long long)v);
    for (size_t i = 0; i < sizeof(T); ++i) printf("%02X ", b[i]);
    printf("\n");
}

int main() {
    printf("===== ① 基本单位 =====\n");
    printf("  1 字节(byte) = 8 位(bit)  ⇒ unsigned char 的取值范围 0..%d（=0xFF）\n", UCHAR_MAX);
    printf("  sizeof(char)=%zu  sizeof(int)=%zu  sizeof(long long)=%zu\n",
           sizeof(char), sizeof(int), sizeof(long long));

    printf("\n===== ② memset 的第二个参数就是「每个字节要填的值」 =====\n");
    printf("  memset 的第二个参数 1002 → 先截成 1 个字节：低 8 位 = %d = 0x%02X\n",
           1002 & 0xFF, 1002 & 0xFF);
    printf("  1002 & 0xFF = %d      （0x%X，二进制 ", 1002 & 0xFF, 1002 & 0xFF);
    for (int i = 7; i >= 0; --i) printf("%d", (1002 >> i) & 1);
    printf("）\n");

    printf("\n===== ③ 同一个字节值，填进不同宽度的类型会变成什么 =====\n");
    printf("  %-10s %-8s %-22s %s\n", "第二参数", "每字节", "int（4 字节）", "long long（8 字节）");
    int second[] = {0, -1, 1, 0x3f, 0x7f, 0x80, 1002, 255, 256};
    for (int s : second) {
        int ai[1]; long long al[1];
        memset(ai, s, sizeof(ai));
        memset(al, s, sizeof(al));
        printf("  %-10d 0x%02X     %-22d %lld\n", s, (unsigned char)s, ai[0], al[0]);
    }

    printf("\n===== ④ 把几个数按字节摊开看（本质：一个数就是若干字节拼起来的）=====\n");
    dumpBytes("int 0x12345678", 0x12345678);
    dumpBytes("int 1", 1);
    dumpBytes("int -1", -1);
    dumpBytes("long long -1", -1LL);
    dumpBytes("long long 0x3f", 0x3F3F3F3F3F3F3F3FLL);
    printf("  ↑ 这就是「小端序」：低位字节放在低地址。x86/评测机都是这个顺序。\n");

    printf("\n===== ⑤ 为什么只有 0 / -1 / 0x3f / 0x7f 能用 memset 填？=====\n");
    printf("  只有这几个值的 8 个二进制位【全都一样】，所以 4 字节、8 字节重复出来还是「同一个数」：\n");
    printf("    0    = 0x00 → 0x00000000 / 0x0000000000000000            = 0\n");
    printf("    -1   = 0xFF → 0xFFFFFFFF / 0xFFFFFFFFFFFFFFFF            = -1\n");
    printf("    0x3f = 0x3F → 0x3F3F3F3F / 0x3F3F3F3F3F3F3F3F = %d / %lld\n",
           (int)0x3F3F3F3F, (long long)0x3F3F3F3F3F3F3F3FLL);
    printf("    0x7f = 0x7F → 0x7F7F7F7F / 0x7F7F7F7F7F7F7F7F = %d / %lld\n",
           (int)0x7F7F7F7F, (long long)0x7F7F7F7F7F7F7F7FLL);
    printf("  而 1002 的低字节是 0xEA → 0xEAEAEAEA (int) / 0xEAEAEAEAEAEAEAEA (ll)，都是【负数】\n");
    return 0;
}
