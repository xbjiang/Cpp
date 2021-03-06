#include <iostream>
using namespace std;

int& test();
extern int a; // global variable has to be declared with extern again here

//extern static int b; // wrong, static variable can not be external

class A
{
public:
	const static int a = 1; // why this has to be const? in-class initialization for static members must be static const int.
	static int b;
    int c = 1;
};
int A::b = 2; // when this variable is initialized? first time you encounter it. and it's decided in compile time

int main()
{
	cout << a << endl;
	int& r_c = test(); // you can return a reference to a local static variable
	r_c = 2;
	cout << "main() c = " << r_c << endl;
	test();
	cout << "A::a " << A::a << endl;
	cout << "A::b " << A::b << endl;
   
}

