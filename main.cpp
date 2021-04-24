#include <iostream>
#include <matrixgraph.h>
#include <listgraph.h>
#include <binaryheap.h>
using namespace std;

BinaryHeap bHeap;
MatrixGraph matrix;

int main()
{
    string tmp;
    cout<<"Wprowadz plik"<<endl;;
    cin>>tmp;
    matrix.setDirected(true);
    matrix.readFromFile(tmp);
    matrix.print();
    matrix.bellmanFordAlg(0);

    return 0;
}

