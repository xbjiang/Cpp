#ifndef __SINGLETON1_H__
#define __SINGLETON1_H__

#include <iostream>
using namespace std;

class Singleton1
{
public:
	static Singleton1* GetInstance()
	{
		if (m_Instance == nullptr)
		{
			m_Instance = new Singleton1();
		}
		return m_Instance;
	}

	static void DestroyInstance()
	{
		if (m_Instance != nullptr)
		{
			delete m_Instance;
			m_Instance = nullptr;
		}
	}

	int GetTest()
	{
		return m_test;
	}

private:
	static Singleton1* m_Instance;
	int m_test;

	Singleton1() : m_test(10) {}
};

#endif