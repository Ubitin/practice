/*作用：执行多条件分支语句
语法：
switch(表达式)

{

case 结果1：执行语句；break;
case 结果2：执行语句；break;

...
default:执行语句；break;
}*/

#include<iostream>
using namespace std;
int main() {

	//给一个电影打分
	//10~9  经典
	//8~7   非常好
	//6~5   一般
	//5以下 烂片

	int score = 0;
	cout << "请为影片评分：" << endl;
	cin >> score;
	cout << "您为影片的评分为："<<score << endl;

	switch (score)//表达式类型只能是整型或者字符型
	{
	case 10://代表一个分支
		cout << "您认为是经典电影" << endl;
		break;//代表退出当前分支，否则会一直向下执行
	case 9:
		cout << "您认为是经典电影" << endl;
		break;
	case 8:
		cout << "您认为电影非常好" << endl;
		break;
	case 7:
		cout << "您认为电影非常好" << endl;
		break;
	case 6:
		cout << "您认为电影一般" << endl;
		break;
	case 5:
		cout << "您认为电影一般" << endl;
		break;
	default:
		cout << "您认为是烂片" << endl;
		break;
	}

	system("pause");
	return 0;
}
//if和switch的区别：
//switch的缺点：判断的时候只能是整型或者字符型，不可以是一个区间
//优点：结构清晰，执行效率高