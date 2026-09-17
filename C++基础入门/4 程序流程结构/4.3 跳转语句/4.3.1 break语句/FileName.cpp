/*作用：用于跳出选择结构或者循环结构
break使用的时机：
·出现在switch条件语句中，作用是终止case并跳出switch
·出现在循环语句中，作用是跳出当前的循环语句
·出现在嵌套循环中，跳出最近的内层循环语句*/

#include<iostream>
using namespace std;
int main() {
	//1、在switch语句中使用break

	cout << "请选择副本的难度" << endl;
	cout << "1、普通" << endl;
	cout << "2、中等" << endl;
	cout << "3、困难" << endl;

	int select = 0;//创建选择结果的变量
	cin >> select;//等待用户输入

	switch (select)
	{
	case 1:
		cout << "您选择的是普通难度" << endl;
		break;
	case 2:
		cout << "您选择的是中等难度" << endl;
		break;
	case 3:
		cout << "您选择的是困难难度" << endl;
		break;
	default:
		break;
	}

	//2、在循环语句中使用break

	for (int i = 0;i <= 9;i++)
	{
		cout << i << endl;
		if (i == 5)
		{
			break;
		}
	}

	//3、在嵌套循环中使用break

	for (int l = 0;l < 11;l++)
	{
		for (int j = 0;j < 11;j++)
		{
			cout << "* ";
			if (j == 5)
			{
				break;
			}
		}
		cout << endl;
	}

	system("pause");
	return 0;
}