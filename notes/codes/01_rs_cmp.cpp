#include <bits/stdc++.h>
using namespace std;
int main(){
    srand(42);
    vector<int> yours(100, 0);
    int rejected = 0;
    for(int i = 0; i < 100000; i++){
        int res; do{ res = rand(); rejected++; } while(res > RAND_MAX % 100);
        yours[res % 100]++;
    }
    int hi_cnt = 0; for(int i = 68; i < 100; i++) hi_cnt += yours[i];
    cout << "你的逻辑: 0~67 合计 " << yours[0]+yours[67] << " 次, 68~99 合计 " << hi_cnt
         << " 次(应为约 32000), 拒绝率 " << (rejected - 100000) * 100.0 / 100000 << "%\n";

    vector<int> correct(100, 0);
    int k = (RAND_MAX + 1) / 100 * 100;
    int rej2 = 0;
    for(int i = 0; i < 100000; i++){
        int res; do{ res = rand(); rej2++; } while(res >= k);
        correct[res % 100]++;
    }
    int mn2 = *min_element(correct.begin(), correct.end()), mx2 = *max_element(correct.begin(), correct.end());
    cout << "正确版: 100 值全覆盖, 最小=" << mn2 << " 最大=" << mx2
         << " (理论各 1000), 拒绝率 " << (rej2 - 100000) * 100.0 / 100000 << "%\n";
    return 0;
}
