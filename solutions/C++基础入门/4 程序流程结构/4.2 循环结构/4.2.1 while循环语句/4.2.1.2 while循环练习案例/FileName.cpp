//案例描述：系统随机生成一个1到100之间的数字，玩家进行猜测，若猜错，提示玩家数字过大或过小，若猜对，则恭喜玩家胜利并退出游戏

#include<iostream>
using namespace std;
//time系统时间头文件包含：
#include<ctime>

int main() {

	//添加随机数种子  作用：利用当前系统的时间生成随机数，防止每次随机数都一样
	srand((unsigned int)time(NULL));

	int num = rand() % 100 + 1;//rand()%100  生成0~99的随机数，所以只需要在后面加1
	int val = 0;//玩家输入的数据
	cin >> val;
	while (1)
	{
		if (val > num)
		{
			cout << "您猜大了" << endl;
		}
		else if (val < num)
		{
			cout << "您猜小了" << endl;
		}
		else
		{
			cout << "恭喜您猜对了" << endl;
			break;//break,可以利用该关键字来退出当前循环
		}
		cin >> val;
	}
	system("pause");
	return 0;
}