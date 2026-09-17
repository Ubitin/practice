//作用：用于表示小数
//浮点型变量分为两种：
//1 单精度float
//2 双精度double
//两者的区别在于表示的有效数字范围不同
/*数据类型                    占用空间                      有效数字范围
  float                       4字节                         7位有效数字
  double                      8字节                         15~16位有效数字*/
//结论：默认情况下，输出一个小数，会显示出6位有效数字
#include<iostream>;
using namespace std;
int main() {
	
	float f1 = 3.1415926f;//为什么是“3.14f”？因为若是“3.14”就默认是双精度的数据，要用字母“f”告诉电脑这是一个单精度的数据
	cout << "f1 = " << f1 << endl;
	
	double d1 = 3.1415926;
	cout << "d1 = " << d1 << endl;

	//统计float和double占用的内存空间
	cout << "float占用的内存空间为" << sizeof(float) << endl;
	cout << "double占用的内存空间为" << sizeof(double) << endl;
	
	//科学计数法
	float f2 = 3e2;//3*10^2
	cout << "f2 = "<<f2 << endl;

	float f3 = 3e-2;//3*10^(-2)
	cout << "f3 = " << f3 << endl;

	system("pause");
	return 0;
}