#include <vector>
#include <list>
#include <iostream>
using namespace std;

struct EdgeNode
{
    int id;
    int weight;
};
struct VertexNode 
{
    char name;
    list<EdgeNode> adjList;
};

int main()
{
    vector<int> a;
    for (int i = 0; i < 10; i++)
    {
        cout << sizeof(a) << endl;
        a.push_back(i);
    }
    cout << sizeof(int) << endl;
    cout << sizeof(long long) << endl;
    vector<long> b;
    cout << sizeof(b) << endl;

    list<int> c;
    cout << sizeof(c) << endl;
    
    cout << sizeof(EdgeNode) << endl;
    cout << sizeof(VertexNode) << endl;
}
