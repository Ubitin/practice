/*作用：在循环体中再嵌套一层循环，解决一些实际问题*/

#include<iostream>
using namespace std;
int main() {
	//外层执行一次，内层执行一周（期）
	//外层循环
	for (int i = 0;i < 10;i++)
	{
		//内层循环
		for (int j = 0;j < 10;j++)
		{
			cout << "* ";
		}
		cout << endl;
	}

	system("pause");
	return 0;
}