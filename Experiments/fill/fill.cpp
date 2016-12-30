#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    bool* visited = new bool[10];
    for (int i = 0; i < 10; i++)
        cout << visited[i] << " ";
    cout << endl;
    fill(visited, visited + 10, true);
    for (int i = 0; i < 10; i++)
        cout << visited[i] << " ";
    cout << endl;
}