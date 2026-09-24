// 「字节值」相关的代码实现 —— 四件套工具 + 穷举验证
// 编译: g++ -O2 -std=c++17 -o byte_convert byte_convert.cpp
#include <bits/stdc++.h>
using namespace std;

// ============ 工具①：取一个整数 v 的「字节值」（低 8 位）============
inline unsigned char lowByte(long long v) {
    return (unsigned char)(v & 0xFF);        // 等价 (unsigned char)v；⚠️ 别用 v % 256（负数错）
}

// ============ 工具②：字节值 b → 某种宽度被 memset 填出来的完整数值 ============
// 用 unsigned 版本左移拼接，最后 memcpy 回 T —— 避免有符号左移溢出（UB）与实现定义转换
template <class T>
T fillPattern(unsigned char b) {
    using U = make_unsigned_t<T>;
    U u = 0;
    for (size_t i = 0; i < sizeof(T); ++i) u = (U)((u << 8) | (U)b);
    T out;
    memcpy(&out, &u, sizeof(T));
    return out;
}
// 特例（手写更快）：int → 0x01010101u * b ；long long → 0x0101010101010101ULL * b
inline int      fillPatternInt(unsigned char b) { return (int)(0x01010101u * (unsigned)b); }
inline long long fillPatternLL (unsigned char b) { return (long long)(0x0101010101010101ULL * (unsigned long long)b); }

// ============ 工具③：判断某个值能不能用 memset 安全填出来（所有字节相同？）============
template <class T>
bool memsetSafe(long long v) {
    T x;
    memcpy(&x, &v, sizeof(T) < sizeof(v) ? sizeof(T) : sizeof(v));   // 取低 sizeof(T) 字节
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&x);   // 读对象表示是标准允许的
    for (size_t i = 1; i < sizeof(T); ++i)
        if (p[i] != p[0]) return false;
    return true;
}

// ============ 工具④：反推——想让 memset 填出 want，该填哪个字节？做不到返回 -1 ============
template <class T>
int patternFor(T want) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&want);
    for (size_t i = 1; i < sizeof(T); ++i) if (p[i] != p[0]) return -1;
    return p[0];
}

// ============ 附：把任意对象的每个字节打印出来（调试用）============
template <class T>
void dumpBytes(const char* name, const T& x) {
    const unsigned char* p = reinterpret_cast<const unsigned char*>(&x);
    printf("  %-22s 字节: ", name);
    for (size_t i = 0; i < sizeof(T); ++i) printf("%02X ", p[i]);
    printf("\n");
}
string toBin(unsigned char b) { string s; for (int i = 7; i >= 0; --i) s += char('0' + ((b >> i) & 1)); return s; }

int main() {
    printf("===== ① 工具① 验证：lowByte 与 (unsigned char)v、与真实 memset 字节一致 =====\n");
    int bad = 0;
    for (long long v = -300; v <= 300; ++v) {
        int arr[1];
        memset(arr, (int)v, sizeof(arr));
        unsigned char realByte = reinterpret_cast<unsigned char*>(arr)[0];
        if (lowByte(v) != realByte) ++bad;
    }
    printf("  v ∈ [-300, 300] 共 601 个：lowByte(v) 与内存里真实字节不一致 %d 个 %s\n\n",
           bad, bad == 0 ? "✔" : "✘");

    printf("===== ② 工具② 验证：fillPattern<T>(b) 与真实 memset 逐宽度对比 =====\n");
    bad = 0;
    for (int b = 0; b < 256; ++b) {
        int ai[1]; long long al[1]; short as[1]; unsigned int au[1];
        memset(ai, b, sizeof(ai)); memset(al, b, sizeof(al));
        memset(as, b, sizeof(as)); memset(au, b, sizeof(au));
        if (fillPattern<int>(b) != ai[0]) ++bad;
        if (fillPattern<long long>(b) != al[0]) ++bad;
        if (fillPattern<short>(b) != as[0]) ++bad;
        if (fillPattern<unsigned int>(b) != au[0]) ++bad;
        if (fillPatternInt(b) != ai[0]) ++bad;
        if (fillPatternLL(b) != al[0]) ++bad;
    }
    printf("  256 个字节值 × (int / ll / short / unsigned / 两个手写特例) 共 1536 组：不一致 %d 组 %s\n",
           bad, bad == 0 ? "✔" : "✘");
    printf("  抽样：b=0xEA → int=%d, ll=%lld ; b=0x3F → int=%d, ll=%lld\n\n",
           fillPattern<int>(0xEA), fillPattern<long long>(0xEA),
           fillPattern<int>(0x3F), fillPattern<long long>(0x3F));

    printf("===== ③ 工具③④：哪些值能用 memset 填？反推该填哪个字节 =====\n");
    printf("  %-24s %-22s %s\n", "想填充的值(int)", "memsetSafe<int>?", "patternFor<int> → 该填的字节");
    int probes[] = {0, 1, 16843009, -1, 1061109567, 2139062143, -353703190, 1002, 12345};
    for (int v : probes) {
        int b = patternFor<int>(v);
        char buf[64];
        if (b < 0) snprintf(buf, sizeof buf, "不可（必须用 fill/循环）");
        else       snprintf(buf, sizeof buf, "%d (0x%02X)", b, (unsigned char)b);
        printf("  %-24d %-22s %s\n", v, memsetSafe<int>(v) ? "是" : "否 ✘", buf);
    }
    printf("  ↑ 规律：只有 0x00000000 / 0x01010101 / 0x3F3F3F3F / 0x7F7F7F7F / 0xFFFFFFFF … 这类【每个字节都一样】的值才行\n\n");

    printf("===== ④ 穷举：memsetSafe<int> 的判定与「4 个字节是否相同」完全等价？=====\n");
    bad = 0; long long yes = 0;
    for (long long v = -200000; v <= 200000; ++v) {
        int x = (int)v;
        const unsigned char* p = reinterpret_cast<const unsigned char*>(&x);
        bool brute = (p[0] == p[1] && p[1] == p[2] && p[2] == p[3]);
        bool tool = memsetSafe<int>(v);
        if (brute != tool) ++bad;
        if (tool) ++yes;
    }
    printf("  400001 个值：不一致 %d 个 %s（其中 %lld 个是「可 memset 填」的）\n\n",
           bad, bad == 0 ? "✔" : "✘", yes);

    printf("===== ⑤ 调试用：打印字节 =====\n");
    int a = 1002; long long c = -1; int d = fillPattern<int>(0x3F);
    dumpBytes("int 1002", a);
    dumpBytes("long long -1", c);
    dumpBytes("int 0x3F3F3F3F", d);
    printf("  字节值 0xEA 的二进制 = %s\n", toBin(0xEA).c_str());

    printf("\n===== ⑥ 实战判据：什么时候必须放弃 memset =====\n");
    printf("  想填的值不是「每个字节都一样」⇒ 用 fill 或循环，别用 memset：\n");
    printf("    fill(&a[0][0], &a[0][0] + N * M, INF);      // 一维/摊平的二维数组\n");
    printf("    for (i) for (j) a[i][j] = INF;               // 最直白\n");
    printf("    vector: std::fill(v.begin(), v.end(), INF);  // 容器用 fill\n");
    printf("  判据代码：if (patternFor<int>(INF) < 0) 用 fill;   // 反推不出字节 ⇒ 不能 memset\n");
    return 0;
}
