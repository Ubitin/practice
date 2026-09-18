/*查看二维数组所占内存空间
获取二维数组首地址*/

#include<iostream>
using namespace std;
int main() {

	int arr[2][3] =
	{
		{1,2,3},
		{4,5,6}
	};
	cout << "二维数组大小：" << sizeof(arr) << endl;

	cout << "二维数组一行大小：" << sizeof(arr[0]) << endl;

	cout << "二维数组一个元素大小：" << sizeof(arr[0][0]) << endl;

	cout << "二维数组行数为：" << sizeof(arr) / sizeof(arr[0]) << endl;

	cout << "二维数组列数为：" << sizeof(arr[0]) / sizeof(arr[0][0]) << endl;

	//获取首地址
	cout << "二维数组的首地址为：" << (int)arr << endl;
	cout << "二维数组第一行首地址：" << (int)arr[0] << endl;
	cout << "二维数组第二行首地址为：" << (int)arr[1] << endl;
	cout << "二维数组第一个元素首地址：" << (int)&arr[0][0] << endl;
	system("pause");
	return 0;
}