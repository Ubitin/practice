//场景描述：从1开始数到100，若数字个位或十位含有7，或该数字是7的倍数，我们打印“敲桌子”，其余数字直接打印输出

#include<iostream>
using namespace std;
int main() {
	for (int num = 1;num <= 100;num++)
	{
		int a = 0;
		int b = 0;
		a = num / 10;
		b = num % 10;
		if ((a == 7) || (b == 7) || (num % 7 == 0))
		{
			cout << "敲桌子" << endl;
		}
		else
		{
			cout << num << endl;
		}
	}
	system("pause");
	return 0;
}