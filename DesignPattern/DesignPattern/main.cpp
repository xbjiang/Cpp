#include <iostream>
#include "Singleton1.h"

using namespace std;

int main(int argc, char* argv[])
{
	Singleton1* p_instance = Singleton1::GetInstance();
	cout << p_instance->GetTest() << endl;
}
