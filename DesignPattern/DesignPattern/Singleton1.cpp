#include <iostream>
#include <stdio.h>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

class Singleton1
{
public:
	static Singleton1* GetInstance()
	{
		if (m_Instance == nullptr)
		{
            m_Mutex.lock();
            if (m_Instance == nullptr)
            {
                m_Instance = new Singleton1();
            }
            m_Mutex.unlock();
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
    static mutex m_Mutex;

    Singleton1() : m_test(10) { printf("Initialization\n"); }
};

Singleton1* Singleton1::m_Instance = nullptr;
mutex Singleton1::m_Mutex;

mutex g_Mutex;
void test()
{
    Singleton1* p_instance = Singleton1::GetInstance();
    //printf("thread %d: %d\n", this_thread::get_id(), p_instance->GetTest());
    g_Mutex.lock();
    cout << p_instance->GetTest() << endl;
    g_Mutex.unlock();
}

int main(int argc, char* argv[])
{
    vector<thread> threads;
    for (int i = 0; i < 10000; i++)
        threads.push_back(thread(test));

    for (auto& t : threads)
        t.join();
}