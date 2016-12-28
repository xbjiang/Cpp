#include <iostream>

using namespace std;

int main()
{
    int** a; 
    a = new int*[10];
    for (int i = 0; i < 10; i++)
        a[i] = new int[10];
    

    int** b;
    b = new int*[10];
    for (int i = 0; i < 10; i++)
        b[i] = new int[10]();

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            cout << a[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
            cout << b[i][j] << " ";
        cout << endl;
    }
    
    bool* visited = new bool[10]();
    cout << sizeof(bool) << endl;
    for (int i = 0; i < 10; i++)
        cout << visited[i] << " ";
    cout << endl;
}