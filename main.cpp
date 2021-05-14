#include <iostream>
#include "matrixgraph.h"
#include "listgraph.h"
#include "binaryheap.h"
#include <windows.h>
#include <iomanip>

using namespace std;

long long int read_QPC()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}

MatrixGraph matrix;
ListGraph listg;

int main()
{
    long long int frequency, start, elapsed;
    QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);

    /* Nie zapominac o setDirect dla algorytmow MST oraz sciezki */

   // listg.readFromFile(tmp);

   // listg.dijkstraAlg(0);
    double sum = 0;
    int vertex = 2000;
    for(int i=0;i<2;i++) {
        int maxEdges = vertex*(vertex-1)/2;
        listg.createRandomGraph(vertex, true, (int)(maxEdges*0.25));
        cout<<"Graph 25 created"<<endl;
        start = read_QPC();
        listg.dijkstraAlg(0);
        elapsed = read_QPC() - start;
        sum+= elapsed;
    }
    cout << "Time [ms] 25% = "<< (1000.0 * sum) /(frequency*2) << endl;

    sum = 0;
    for(int i=0;i<2;i++) {
        int maxEdges = vertex*(vertex-1)/2;
        listg.createRandomGraph(vertex, true, (int)(maxEdges*0.5));
     cout<<"Graph 50 created"<<endl;
        start = read_QPC();
        listg.dijkstraAlg(0);
        elapsed = read_QPC() - start;
        sum+= elapsed;
    }
    cout << "Time [ms] 50% = "<< (1000.0 * sum) /(frequency*2) << endl;

    sum = 0;
    for(int i=0;i<2;i++) {
        int maxEdges = vertex*(vertex-1)/2;
        listg.createRandomGraph(vertex, true, (int)(maxEdges*0.75));
 cout<<"Graph 75 created"<<endl;
        start = read_QPC();
        listg.dijkstraAlg(0);
        elapsed = read_QPC() - start;
        sum+= elapsed;
    }
    cout << "Time [ms] 75% = "<< (1000.0 * sum) /(frequency*2) << endl;

    sum = 0;
    for(int i=0;i<2;i++) {
        int maxEdges = vertex*(vertex-1)/2;
        listg.createRandomGraph(vertex, true, (int)(maxEdges*0.99));
 cout<<"Graph 99 created"<<endl;
        start = read_QPC();
        listg.dijkstraAlg(0);
        elapsed = read_QPC() - start;
        sum+= elapsed;
    }
    cout << "Time [ms] 99% = "<< (1000.0 * sum) /(frequency*2) << endl;
    return 0;
}

