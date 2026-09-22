#include <bits/stdc++.h>
using namespace std;

// 造几个"结构化"大用例，专门把 BFS 逼到网格边界（x/y 到 309 再往外迈）
int main(int argc, char** argv) {
    int kind = argc > 1 ? atoi(argv[1]) : 0;
    if (kind == 0) {          // 一颗陨石扔在角落：起点附近全空 → BFS 铺满整个网格
        printf("1\n300 300 1000\n");
    } else if (kind == 1) {   // 全部陨石砸在 (300,300)，同样是铺满
        printf("50000\n");
        for (int i = 0; i < 50000; ++i) printf("300 300 1000\n");
    } else if (kind == 2) {   // 沿两条边造"晚爆墙"，把 BFS 逼着往远处绕
        printf("%d\n", 602);
        for (int i = 0; i <= 300; ++i) printf("%d 300 1000\n", i);
        for (int i = 0; i <= 300; ++i) printf("300 %d 1000\n", i);
    } else if (kind == 3) {   // 起点四邻很晚才炸 → BFS 长期在自由区扩散
        printf("5\n1 0 999\n0 1 999\n100 100 1000\n200 200 1000\n300 300 1000\n");
    } else if (kind == 4) {   // 空场（M 很小、都在远处）
        printf("2\n299 299 1000\n300 0 1000\n");
    }
    return 0;
}
