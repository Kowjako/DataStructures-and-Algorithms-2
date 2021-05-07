#include <iostream>
#include <matrixgraph.h>
#include <listgraph.h>
#include <binaryheap.h>
using namespace std;

MatrixGraph matrix;
ListGraph listg;
BinaryHeap b;
int main()
{
    /* Nie zapominac o setDirect dla algorytmow MST oraz sciezki */
    string tmp;
    cout<<"Wprowadz plik"<<endl;;
    cin>>tmp;
    listg.createRandomGraph(20,true,40);
    listg.print();
    cout<<listg.getEdgeNumber()<<endl;
    cout<<listg.getNodeNumber()<<endl;
    return 0;
}

