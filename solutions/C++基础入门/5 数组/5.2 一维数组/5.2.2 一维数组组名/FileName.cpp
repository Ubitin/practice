/*数组名称的作用：
1、可以统计整个数组在内存中的长度  sizeof(arr)   sizeof(arr[i])  sizeof(arr)/sizeof(arr[i]=数组元素个数
2、可以获取数组在内存中的首地址    cout<<arr<<endl;一般为16进制*/

#include<iostream>
using namespace std;
int main() {

	//1、
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	cout << "整个数组占用的内存空间为：" << sizeof(arr) << endl;//4*10
	cout << "每个元素占用的内存空间为：" << sizeof(arr[0]) << endl;//int(整型）4个字节
	cout << "数组中元素的个数为：" << sizeof(arr) / sizeof(arr[1]) << endl;

	//2、
	cout << arr << endl;//输出为16进制
	cout << (int)arr << endl;//将16进制强行转换为10进制
	cout << "数组中第一个元素的地址为：" << (int)&arr[0] << endl;
	//整个数组的首地址与数组中第一个元素的首地址一样
	cout << "数组中第二个元素的地址为：" << (int)&arr[1] << endl;

	system("pause");
	return 0;
}

//注意：数组名是常量，不可以进行赋值操作