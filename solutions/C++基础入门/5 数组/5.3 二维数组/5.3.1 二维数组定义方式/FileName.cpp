/*四种：（以矩阵的形式呈现）
1、数据类型 数组名[行数][列数]；
2、数据类型 数组名[行数][列数]={{数据1，数据2}，{数据3，数据4}}；
3、数据类型 数组名[行数][列数]={数据1，数据2，数据3，数据4}；
4、数据类型 数组名[][列数]={数据1，数据2，数据3，数据4}；
tips:利用第二种更加直观*/

#include<iostream>
using namespace std;
int main() {

	//1.
	int arr1[2][3];
	arr1[0][0] = 1;
	arr1[0][1] = 2;
	arr1[0][2] = 3;
	arr1[1][0] = 4;
	arr1[1][1] = 5;
	arr1[1][2] = 6;

	for(int i=0;i<2;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			cout << arr1[i][j] << endl;
		}
	}

	//2.
	int arr2[2][3] =
	{
		{1,2,3},
		{4,5,6}
	};
	for (int i = 0;i < 2;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			cout << arr2[i][j] << " ";
		}
		cout << endl;
	}

	//3.
	int arr3[2][3] = { 1,2,3,4,5,6 };//自动帮你分好行和列
	for (int i = 0;i < 2;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			cout << arr3[i][j] << " ";
		}
		cout << endl;
	}

	//4.
	int arr4[][3] = { 1,2,3,4,5,6 };//因为已经是三列了，所以系统可以自己推算出来有两行
	for (int i = 0;i < 2;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			cout << arr4[i][j] << " ";
		}
		cout << endl;
	}

	system("pause");
	return 0;
}