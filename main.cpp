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
    listg.setDirected(true);
    listg.readFromFile(tmp);
    listg.print();
    listg.dijkstraAlg(0);
    return 0;
}

