//案例描述：利用嵌套循环，实现九九乘法表

#include<iostream>
using namespace std;
int main() {
	int num = 0;
	for (int i = 1;i <= 9;i++)
	{
		for (int j = 1;j <= i;j++)
		{
			num = i * j;
			cout << j << "*" << i << "=" << num << "\t";
		}
		cout << endl;
	}
	system("pause");
	return 0;
}