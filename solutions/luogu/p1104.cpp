#include <bits/stdc++.h>
using namespace std;
int n;
struct node
{
	string s;
	int id, y, m, d;
} a[110];
bool cmp(node a, node b)
{
	if (a.y == b.y)
	{
		if (a.m == b.m)
        {
            if (a.d == b.d)
                return a.id > b.id;
            return a.d < b.d;
        }
		return a.m < b.m;
	}
	return a.y < b.y;
}
int main()
{
	ios::sync_with_stdio(0), cin.tie(0); 
    cin >> n;
    for (int i = 1; i <= n; i++)
    {
        cin >> a[i].s >> a[i].y >> a[i].m >> a[i].d;
        a[i].id = i;
    }
    sort(a + 1, a + n + 1, cmp);
    for (int i = 1; i <= n; i++)
    	cout << a[i].s << "\n";
   	return 0;
}