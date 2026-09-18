//案例描述：请声明一个五个元素的数组，并将元素逆置

#include<iostream>
using namespace std;
int main() {

	int arr[5] = { 1,3,4,5,2 };
	
	cout << "数组逆置前：" << endl;
	for (int j = 0;j < 5;j++)
	{
		cout <<  arr[j] << endl;
	}

	int temp = 0;//临时储存空间
	int start = 0;//起始元素下标
	int end = sizeof(arr) / sizeof(arr[0])-1;//末尾元素下标
	
	for (;start < end;start++, end--)//互换操作
	{
		temp = arr[start];
		arr[start] = arr[end];
		arr[end] = temp;
	}
	cout << "数组逆置后:" << endl;
	for (int i = 0;i < (sizeof(arr)/sizeof(arr[start]));i++)
	{
		cout << arr[i] << endl;
	}

	system("pause");
	return 0;
}