#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin >> n;
    int cnt[4] = {0};
    for (int i = 0; i < n; i++) {
        int age; cin >> age;
        if (age >= 0 && age <= 18) cnt[0]++;
        else if (age <= 35) cnt[1]++;
        else if (age <= 60) cnt[2]++;
        else cnt[3]++;
    }
    const char* label[4] = {"1-18", "19-35", "36-60", "61-"};
    cout << fixed << setprecision(2);
    for (int j = 0; j < 4; j++)
        cout << label[j] << ": " << (double)cnt[j] / n * 100 << "%\n";
    return 0;
}
