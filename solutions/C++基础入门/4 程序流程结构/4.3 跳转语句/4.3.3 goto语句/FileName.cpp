/*作用：可以无条件跳转语句
语法：goto 标记；标记可以随意取名，eg.FLAG
解释：若标记的名称存在，执行到goto语句时，会自动跳转到标记的位置
不推荐使用，影响代码结构和逻辑*/

#include<iostream>
using namespace std;
int main() {

	cout << "1、xxx" << endl;

	cout << "2、xxx" << endl;

	cout << "3、xxx" << endl;

	goto FLAG;

	cout << "4、xxx" << endl;

	cout << "5、xxx" << endl;

	FLAG:

	cout << "6、xxx" << endl;


	system("pause");
	return 0;
}