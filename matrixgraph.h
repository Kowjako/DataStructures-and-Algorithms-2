#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include <iostream>
#include <fstream>
#include <stack>
#include "listgraph.h"
#include <algorithm>

using namespace std;

struct edge {
    int start;
    int finish;
    int weight;
    bool operator()(const edge& first, const edge& second) const { first.weight < second.weight; }
};

class MatrixGraph
{
    public:
        MatrixGraph();
        MatrixGraph(int vertex, bool isDirected);
        virtual ~MatrixGraph();

        void print();

        bool connect(int startVertex, int endVertex, int edge);
        bool disconnect(int startVertex, int endVertex);

        int getEdgeNumber();
        int getNodeNumber();

        /** Algorytmy **/
        void dijkstraAlg(int start);
        void bellmanFordAlg(int start);
        void primAlg(int start);
        void kruskalAlg(int start);

        void MakeSet(); /*tworzenie poddrzewa wierzcholka */

        edge** createSortedEdgesList(); /* uzyte w algorytmie Kruskala */

        bool readFromFile(string filename);
        int getWeight(int start, int finish); /*wyszukanie wagi krawedzi z macierzy sasiedztwa */

        void clear(int n, bool isDirected); /*usuniecie macierzy poprzedniej i tworzenie nowej */
        int** getMacierz();

        int* countNeighbours(int start);

        bool isDirected();
        void setDirected(bool isDirected);
    protected:

    private:
        bool directed; /*graf skierowany lub nie*/
        int** macierz; /*macierz sasiedztwa */
        int** edgeMacierz; /*lista krawedzi*/
        int edge_num;
        int node_num;
        int neighbourCount = 0;
};

#endif // MATRIXGRAPH_H
