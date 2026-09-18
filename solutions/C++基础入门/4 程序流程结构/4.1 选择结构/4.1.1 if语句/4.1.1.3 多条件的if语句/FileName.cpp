//3、多条件的if语句：
//if（条件1）{条件1满足时执行的语句}else if（条件2）{条件2满足时执行的语句}...else{所有条件都不满足时执行的语句}
#include<iostream>
using namespace std;
int main() {

	//场景：输入一个考试的分数，若大于600分，则视为考上一本大学并输出
	//若大于500分，则视为考上二本大学并输出
	//大于400分，则视为考上三本大学并输出
	//小于等于400分，则视为未考上本科并输出

	int score = 0;
	cout << "请输入您的分数：" << endl;
	cin >> score;
	cout << "您的分数为：" << score << endl;
	if (score > 600)
	{
		cout << "恭喜您考上一本大学！" << endl;
	}
	else if (score > 500)
	{
		cout << "恭喜您考上二本大学！" << endl;
	}
	else if (score > 400)
	{
		cout << "恭喜您考上三本大学" << endl;
	}
	else
	{
		cout << "很遗憾，您未能考上大学" << endl;
	}
	system("pause");
	return 0;
}