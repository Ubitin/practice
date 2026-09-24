// 字节值 215 换算成各宽度的值（实测 + 公式对照）
#include <bits/stdc++.h>
using namespace std;

int main() {
    unsigned char b = 215;
    printf("===== 字节值 215 本身 =====\n");
    printf("  十进制 215   十六进制 0x%02X   二进制 ", b);
    for (int i = 7; i >= 0; --i) printf("%d", (b >> i) & 1);
    printf("   权重和 = 128+64+16+4+2+1 = %d\n", 128 + 64 + 16 + 4 + 2 + 1);

    printf("\n===== 用真实 memset 填出来 =====\n");
    int ai[1]; unsigned au[1]; long long al[1]; unsigned long long aul[1]; short as[1]; unsigned short aus[1];
    memset(ai, b, sizeof ai); memset(au, b, sizeof au);
    memset(al, b, sizeof al); memset(aul, b, sizeof aul);
    memset(as, b, sizeof as); memset(aus, b, sizeof aus);
    printf("  int(4 字节)                 = %d        (0x%08X)\n", ai[0], (unsigned)ai[0]);
    printf("  unsigned int(4 字节)        = %u        (0x%08X)\n", au[0], au[0]);
    printf("  long long(8 字节)           = %lld\n", al[0]);
    printf("  unsigned long long(8 字节)  = %llu\n", aul[0]);
    printf("  short(2 字节)               = %d        (0x%04X)\n", (int)as[0], (unsigned)(unsigned short)as[0]);
    printf("  unsigned short(2 字节)      = %u\n", (unsigned)aus[0]);
    printf("  单字节(即字节值本身)        = %u\n", b);

    printf("\n===== 与公式对照（0x01010101u×215 / 0x0101010101010101ULL×215）=====\n");
    int fi = (int)(0x01010101u * (unsigned)b);
    long long fl = (long long)(0x0101010101010101ULL * (unsigned long long)b);
    printf("  int  公式 = %-12d 实测 = %-12d %s\n", fi, ai[0], fi == ai[0] ? "✔" : "✘");
    printf("  ll   公式 = %-21lld 实测 = %-21lld %s\n", fl, al[0], fl == al[0] ? "✔" : "✘");
    printf("  十六进制：int = 0x%08X ；long long = 0x%016llX\n", (unsigned)ai[0], (unsigned long long)al[0]);

    printf("\n===== 顺带：215 与相关字节值的对照 =====\n");
    printf("  %-8s %-8s %-14s %-22s\n", "字节值", "十六进制", "int", "long long");
    for (unsigned char x : {(unsigned char)0, (unsigned char)0x3f, (unsigned char)0x7f,
                            (unsigned char)0x80, (unsigned char)0xd7, (unsigned char)0xea, (unsigned char)0xff}) {
        printf("  %-8u 0x%02X     %-14d %lld\n", x, x,
               (int)(0x01010101u * (unsigned)x), (long long)(0x0101010101010101ULL * (unsigned long long)x));
    }
    return 0;
}
