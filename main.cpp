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
    string tmp;
    cout<<"Wprowadz plik"<<endl;;
    cin>>tmp;
    matrix.setDirected(true);
    matrix.readFromFile(tmp);
    matrix.bellmanFordAlg(0);
    //matrix.print();
    //matrix.createSortedEdgesList();
    /*matrix.print();
    matrix.primAlg(0);
    listg.setDirected(false);
    listg.readFromFile(tmp);
    listg.print(); */


    return 0;
}

