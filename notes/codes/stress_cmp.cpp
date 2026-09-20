// ============================================================
//  stress_cmp.cpp —— C++ 通用对拍器（Windows / MinGW-w64）
//
//  用法： stress_cmp.exe <gen.exe> <sol.exe> <brute.exe> [轮数] [超时ms] [--ws]
//     轮数     默认 1000
//     超时ms   默认 3000（单次运行上限，超了就强杀并判为 TLE）
//     --ws     比较时忽略所有空白字符（多选题/格式宽松时用）
//
//  三个角色（对拍的标准三件套）：
//     gen.exe   —— 数据生成器，读命令行参数作为随机种子，把数据打到 stdout
//     sol.exe   —— 你的解（待测）
//     brute.exe —— 暴力解（保证正确，但很慢；所以数据规模要小）
//
//  对拍器做的事：循环 { 造数据 → 两边各跑一次 → 比较输出 }
//                出错就把数据和双方输出存成 bad_*.txt 并停下
//
//  ⚠️ 为什么不用最简单的 system("sol.exe < in.txt > out.txt")？
//     system() 会一直阻塞，程序死循环时对拍器自己就挂死了。
//     这里用 CreateProcess 拿进程句柄，WaitForSingleObject 带超时，
//     超时就 TerminateProcess 强杀 —— 于是【TLE 也能被测出来】。
// ============================================================
#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

static const char* IN_TXT  = "stress_in.txt";
static const char* SOL_TXT = "stress_sol.txt";
static const char* BRU_TXT = "stress_bru.txt";
static const char* BAD_IN  = "bad_in.txt";
static const char* BAD_SOL = "bad_sol.txt";
static const char* BAD_BRU = "bad_bru.txt";

struct Res {
    int    code  = -1;      // 进程退出码（0 = 正常）
    bool   tle   = false;   // 是否超时被杀
    double ms    = 0;       // 墙钟耗时
    bool   spawn = false;   // 是否连进程都没起来（路径写错等）
};

#ifdef _WIN32
static Res runProc(const string& exe, const string& args,
                   const string& inFile, const string& outFile, int timeoutMs) {
    Res r;
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    string inPath = inFile.empty() ? "NUL" : inFile;
    HANDLE hIn  = CreateFileA(inPath.c_str(),    GENERIC_READ,  FILE_SHARE_READ, &sa,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hOut = CreateFileA(outFile.c_str(),   GENERIC_WRITE, FILE_SHARE_READ, &sa,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hIn == INVALID_HANDLE_VALUE || hOut == INVALID_HANDLE_VALUE) {
        if (hIn  != INVALID_HANDLE_VALUE) CloseHandle(hIn);
        if (hOut != INVALID_HANDLE_VALUE) CloseHandle(hOut);
        r.spawn = true;
        return r;
    }

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdInput  = hIn;
    si.hStdOutput = hOut;
    si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);   // 子程序的 stderr 直接透传

    string cmd = "\"" + exe + "\"";
    if (!args.empty()) cmd += " " + args;
    vector<char> buf(cmd.begin(), cmd.end());
    buf.push_back('\0');

    PROCESS_INFORMATION pi{};
    auto t0 = chrono::steady_clock::now();
    BOOL ok = CreateProcessA(NULL, buf.data(), NULL, NULL, TRUE,
                             CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(hIn);
    CloseHandle(hOut);
    if (!ok) { r.spawn = true; return r; }

    DWORD wr = WaitForSingleObject(pi.hProcess, (DWORD)timeoutMs);
    r.ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    if (wr == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, 1);            // ★ 强杀，防止对拍器被死循环拖死
        WaitForSingleObject(pi.hProcess, 3000);
        r.tle  = true;
        r.code = -999;
    } else {
        DWORD ec = 0;
        GetExitCodeProcess(pi.hProcess, &ec);
        r.code = (int)ec;
    }
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return r;
}
#else
// 非 Windows 的简易回退：用 system()，但没有超时保护
static Res runProc(const string& exe, const string& args,
                   const string& inFile, const string& outFile, int) {
    Res r;
    string cmd = "\"" + exe + "\"";
    if (!args.empty()) cmd += " " + args;
    if (!inFile.empty())  cmd += " < \"" + inFile + "\"";
    cmd += " > \"" + outFile + "\"";
    auto t0 = chrono::steady_clock::now();
    r.code = system(cmd.c_str());
    r.ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    return r;
}
#endif

static string readAll(const string& p) {
    ifstream f(p, ios::binary);
    stringstream ss; ss << f.rdbuf();
    return ss.str();
}

// 输出归一化：默认忽略"行尾空白 + 末尾空行"；--ws 则忽略所有空白
static string normalize(const string& s, bool ignoreWs) {
    if (ignoreWs) {
        string r;
        for (char c : s) if (!isspace((unsigned char)c)) r += c;
        return r;
    }
    vector<string> lines;
    string cur;
    for (char c : s) {
        if (c == '\r') continue;
        if (c == '\n') {
            while (!cur.empty() && isspace((unsigned char)cur.back())) cur.pop_back();
            lines.push_back(cur);
            cur.clear();
        } else cur += c;
    }
    if (!cur.empty()) {
        while (!cur.empty() && isspace((unsigned char)cur.back())) cur.pop_back();
        lines.push_back(cur);
    }
    while (!lines.empty() && lines.back().empty()) lines.pop_back();
    string r;
    for (auto& l : lines) { r += l; r += '\n'; }
    return r;
}

static void saveBad() {
    // 把出错的那组数据与双方输出另存，方便直接复现
    auto cp = [](const char* from, const char* to) {
        ifstream a(from, ios::binary);
        ofstream b(to, ios::binary);
        b << a.rdbuf();
    };
    cp(IN_TXT, BAD_IN);
    cp(SOL_TXT, BAD_SOL);
    cp(BRU_TXT, BAD_BRU);
}

static void showFirstDiff(const string& A, const string& B, int maxLines) {
    auto la = [](const string& s) {
        vector<string> v; string cur;
        for (char c : s) { if (c == '\n') { v.push_back(cur); cur.clear(); } else cur += c; }
        if (!cur.empty()) v.push_back(cur);
        return v;
    };
    auto va = la(A), vb = la(B);
    printf("  行数：sol=%zu  brute=%zu\n", va.size(), vb.size());
    size_t m = max(va.size(), vb.size());
    int shown = 0;
    for (size_t i = 0; i < m && shown < maxLines; ++i) {
        string x = i < va.size() ? va[i] : string("<缺行>");
        string y = i < vb.size() ? vb[i] : string("<缺行>");
        if (x != y) {
            printf("  第 %zu 行不同：\n    sol   = %s\n    brute = %s\n", i + 1, x.c_str(), y.c_str());
            ++shown;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("用法: stress_cmp.exe <gen.exe> <sol.exe> <brute.exe> [轮数] [超时ms] [--ws]\n");
        printf("  gen 读命令行参数当种子，数据打到 stdout\n");
        printf("  sol 待测解 / brute 暴力解，都从 stdin 读\n");
        return 1;
    }
    string gen = argv[1], sol = argv[2], bru = argv[3];
    int rounds = argc > 4 && argv[4][0] != '-' ? atoi(argv[4]) : 1000;
    int tleMs  = argc > 5 && argv[5][0] != '-' ? atoi(argv[5]) : 3000;
    bool ignoreWs = false;
    for (int i = 1; i < argc; ++i) if (string(argv[i]) == "--ws") ignoreWs = true;

    printf("[对拍] gen=%s\n       sol=%s\n       brute=%s\n", gen.c_str(), sol.c_str(), bru.c_str());
    printf("[对拍] 轮数 %d，单次超时 %d ms，比较模式 %s\n\n",
           rounds, tleMs, ignoreWs ? "忽略全部空白" : "忽略行尾空白");

    int bad = 0, reCnt = 0, tleCnt = 0, genErr = 0;
    double maxSolMs = 0;

    for (int t = 1; t <= rounds; ++t) {
        // ① 造数据（种子用 t，保证可复现）
        Res g = runProc(gen, to_string(t), "", IN_TXT, 10000);
        if (g.spawn || g.tle || g.code != 0) {
            printf("[第 %d 轮] 生成器异常：spawn=%d tle=%d code=%d\n", t, (int)g.spawn, (int)g.tle, g.code);
            if (++genErr >= 3) { printf("生成器连续异常，中止。\n"); return 2; }
            continue;
        }

        // ② 两边各跑一次
        Res s = runProc(sol, "", IN_TXT, SOL_TXT, tleMs);
        Res b = runProc(bru, "", IN_TXT, BRU_TXT, tleMs);
        maxSolMs = max(maxSolMs, s.ms);

        // ③ 先查暴力解是否正常（暴力解自己崩了，对拍就没有意义）
        if (b.spawn || b.tle || b.code != 0) {
            printf("[第 %d 轮] ⚠️ 暴力解异常（spawn=%d tle=%d code=%d）——先修暴力解！\n",
                   t, (int)b.spawn, (int)b.tle, b.code);
            saveBad();
            return 3;
        }

        // ④ 再查待测解的 TLE / RE
        if (s.spawn) { printf("[第 %d 轮] 待测解无法启动（路径写错？）\n", t); return 4; }
        if (s.tle) {
            ++tleCnt;
            printf("[第 %d 轮] ★ TLE：超过 %d ms 被强杀\n", t, tleMs);
            saveBad();
            printf("  → 已存 bad_in.txt / bad_sol.txt / bad_bru.txt\n");
            return 5;
        }
        if (s.code != 0) {
            ++reCnt;
            printf("[第 %d 轮] ★ RE：退出码 %d (0x%08X)\n", t, s.code, (unsigned)s.code);
            saveBad();
            printf("  → 已存 bad_in.txt / bad_sol.txt / bad_bru.txt\n");
            return 6;
        }

        // ⑤ 比输出
        string A = normalize(readAll(SOL_TXT), ignoreWs);
        string B = normalize(readAll(BRU_TXT), ignoreWs);
        if (A != B) {
            ++bad;
            printf("[第 %d 轮] ✗ 输出不一致\n", t);
            showFirstDiff(A, B, 3);
            saveBad();
            printf("  → 已存 bad_in.txt / bad_sol.txt / bad_bru.txt\n");
            printf("  数据（前 400 字符）：\n----\n%s----\n",
                   readAll(BAD_IN).substr(0, 400).c_str());
            return 7;
        }

        if (t % 100 == 0) { printf("  ...已通过 %d 轮\n", t); fflush(stdout); }
    }

    printf("\n[对拍] %d 轮全部通过 ✅\n", rounds);
    printf("       不一致 %d、RE %d、TLE %d、生成器异常 %d\n", bad, reCnt, tleCnt, genErr);
    printf("       待测解单次最慢 %.1f ms（超时阈值 %d ms）\n", maxSolMs, tleMs);
    return 0;
}
