/*作用：最常用的排序算法，对数组内元素进行排序
1、比较相邻的元素。若第一个比第二个大，就交换他们两个。
2、对每一对相邻元素做同样的操作，执行完毕后，找到第一个最大值
3、重复以上步骤，每次比较次数-1，直到不需要比较*/


//1、排序的总轮数为元素的总个数-1 （起始轮数为0）
//2、每轮对比的次数为元素个数-排序轮数-1
//将数组{4，2，8，0，5，7，1，3，9}进行升序排序
#include<iostream>
using namespace std;
int main() {

	int arr[9] = { 4,2,8,0,5,7,1,3,9 };
    
	cout << "排序前：" << endl;
	for (int i = 0;i < 9;i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;

	for (int i = 0;i < 9 - 1;i++)//总共排序的轮数为元素个数-1
	{
		for (int j = 0;j < 9 - i - 1;j++)//内层对比次数为元素个数-排序轮数-1
		{
			if (arr[j] > arr[j + 1])//若前一个元素大于后一个元素，则二者交换顺序
			{
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

	cout << "升序排序后：" << endl;
	for (int k = 0;k < 9;k++)
	{
		cout << arr[k] << " ";
	}
	cout << endl;

	system("pause");
	return 0;
}