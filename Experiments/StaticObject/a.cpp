#include <iostream>
int a;
static int b = 1;

int& test()
{
	static int c = 1;
	std::cout << "test() c = " << c << std::endl;
	return c;
}

