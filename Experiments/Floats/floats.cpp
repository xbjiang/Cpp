#include <iostream>
#include <vector>
using namespace std;

int main()
{
    int a = 1;
    float b = 1.0;
    cout << (a == b) << endl;
    vector<int> vec(3, 0);
    for (int v : vec)
        cout << v << " ";
    cout << endl;
    vec.resize(3, 1);
    for (int v : vec)
        cout << v << " ";
    cout << endl;
}