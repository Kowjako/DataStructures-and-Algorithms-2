#include <iostream>
#include <matrixgraph.h>
#include <listgraph.h>
#include <binaryheap.h>
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

    //matrix.createRandomGraph(5, false, 10);

    matrix.createRandomGraph(5,false,10);

    matrix.print();
    matrix.kruskalAlg();
    /* double sum = 0;
    for(int i=0;i<50;i++) {
        matrix.createRandomGraph(50, false, 306);

        start = read_QPC();
        matrix.kruskalAlg();
        elapsed = read_QPC() - start;
        sum+= elapsed;
    }
    cout << "Time [us] = "<< (1000000.0 * sum) /(frequency*50) << endl; */
    return 0;
}

