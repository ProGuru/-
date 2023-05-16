#include <iostream>
#include "ClassDima.h"

using namespace std;

int main()
{
	double x = 0.0; // Рекомендуется всегда присваивать им некоторые начальные значения
	double y = 0.0; // Рекомендуется всегда присваивать им некоторые начальные значения
	double result = 0.0; // Рекомендуется всегда присваивать им некоторые начальные значения
	char oper = '+'; // Рекомендуется всегда присваивать им некоторые начальные значения

	cout << "Calculator Console Application" << endl << endl;
	cout << "Please enter the operation to perform. Format: a+b | a-b | a*b | a/b" << endl;

	ClassDima c;
	while (true)
	{
		cin >> x >> oper >> y;
		if (oper == '/' && y == 0)
		{
			cout << "Division by 0 exception" << endl;
			continue;
		}
		else
		{
			result = c.ClasstoDima(x, oper, y);
		}
		cout << "Result is: " << result << endl; // что-то там
	}

	return 0; // конец
}