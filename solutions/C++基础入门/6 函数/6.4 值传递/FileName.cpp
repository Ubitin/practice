//所谓值传递，就是函数调用时实参将数值传入形参
//值传递时，若形参发生变化，并不会影响实参

#include<iostream>
using namespace std;
//定义函数 实现两个数字之间的交换
//如果一个函数不需要返回值，声明的时候可以写void

void swap(int num1, int num2)
{
	cout << "交换前：" << endl;
	cout << "num1 = "<< num1  << endl;
	cout << "num2 = "<< num2  << endl;

	int temp = num1;
	num1 = num2;
	num2 = temp;

	cout << "交换后：" << endl;
	cout << "num1 = " << num1 << endl;
	cout << "num2 = " << num2 << endl;

	return;//返回值不需要的时候，可以不写return
}

int main()
{
	//值传递时，形参的变化不影响实参
	int a = 10;
	int b = 20;

	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	swap(a, b);

	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	//a依旧为10，b依旧为20
	system("pause");
	return 0;
}
//原理：实参的值被赋值给了形参，实参与形参各自占有单独的内存空间，当形参进行函数运算时，不影响实参