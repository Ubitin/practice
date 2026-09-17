//案例描述：水仙花数是指一个三位数，它的每个位上的数字的三次幂之和等于它本身
//eg.1^3+5^3+3^3=153
//请利用do...while语句，求出所有三位数中水仙花数的个数

#include<iostream>
using namespace std;
int main() {
	int num = 100;
	int a = 0;
	int c = 0;
	int b = 0;
	a = num / 100;
	b = (num % 100) / 10;
	c = num % 100;
	//cout << a << b << c << endl;
	int comput = 0;

	do {
		if (num == (a*a*a+ b*b*b + c*c*c))
		{
			cout << num << endl;
			comput++;
			num++;
			a = num / 100;
			c = num % 10;
			b = (num % 100) / 10;
		}
		else if (num != (a * a * a + b * b * b + c * c * c))
		{
			num++;
			a = num / 100;
			c = num % 10;
			b = (num % 100) / 10;
		}
	} while (num < 1000);
	cout << comput << endl;

	system("pause");
	return 0;
}