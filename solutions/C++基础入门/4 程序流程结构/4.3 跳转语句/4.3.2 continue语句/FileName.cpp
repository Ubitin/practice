/*作用：在循环语句中，跳过本次循环中余下的尚未执行的语句，继续执行下一次循环
在一个循环里，continue之前的语句都要执行，而continue之后的语句直接跳过，进行下一次循环*/

#include<iostream>
using namespace std;
int main() {
	//输出0~100，且奇数输出，偶数不输出
	for (int i = 0;i <=100;i++)
	{
		if (i % 2 == 0)
		{
			continue;//在实际应用中，continue可以筛选条件，执行到此就不再继续向下执行，执行下一循环，而非break的直接终止循环
		}
		else if (i % 2 == 1)
		{
			cout << i << endl;
		}
	}

	system("pause");
	return 0;
}