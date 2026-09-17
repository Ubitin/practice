/*嵌套if语句：在if语句中，可以嵌套使用if语句，达到更精确的条件判断*/
//场景：1、成绩若大于600分，则视为考上一本；若大于500分考上二本，大于400分考上三本，其余视为未考上大学
//2、在一本分数中，若大于700分视为考上北大，大于650分视为考上清华，大于600分视为考上人大
#include<iostream>
using namespace std;
int main() {

	int score = 0;
	cout << "请输入您的分数：" << endl;
	cin >> score;
	cout << "您的分数为：" << score << endl;
	if (score > 600)
	{
		if (score > 700)
		{
			cout << "恭喜您考上北大" << endl;
		}
		
		else if (score > 650)
		{
			cout << "恭喜您考上清华" << endl;
		}

		else
		{
			cout << "恭喜您考上人大" << endl;
		}
	}

	else if (score > 500)
	{
		cout << "恭喜您考上二本" << endl;
	}

	else if (score > 400)
	{
		cout << "恭喜您考上三本" << endl;
	}

	else
	{
		cout << "很遗憾，您未能考上大学" << endl;
	}

	system("pause");
	return 0;

}