//案例描述：在一个数组中记录了五只小猪的体重，找出并打印最重的小猪的体重

#include<iostream>
using namespace std;
int main()
{
	int arr[5] = { 300,350,200,400,250 };
	int val = 0;
	for (int i = 0;i < 5;i++)
	{
		if (val < arr[i])
		{
			val = arr[i];
		}
		else if (val >= arr[i])
		{
			continue;
		}
	}
	cout << val << endl;
	system("pause");
	return 0;
}