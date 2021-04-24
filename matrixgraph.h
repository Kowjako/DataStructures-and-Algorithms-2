#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include <iostream>
#include <fstream>
#include "listgraph.h"

using namespace std;

class MatrixGraph
{
    public:
        MatrixGraph();
        MatrixGraph(int vertex, bool isDirected);
        virtual ~MatrixGraph();

        void print();

        bool connect(int startVertex, int endVertex, int edge);
        bool disconnect(int startVertex, int endVertex);

        bool isConnected(int x, int y, bool *isVisited);

        int getEdgeNumber();
        int getNodeNumber();

        void dijkstraAlg(int start);
        void bellmanFordAlg(int start);

        bool readFromFile(string filename);
        int getWeight(int start, int finish);

        void clear(int n, bool isDirected); /*usuniecie macierzy poprzedniej i tworzenie nowej */
        int **getMacierz();

        int* countNeighbours(int start);

        bool isDirected();
        void setDirected(bool isDirected);


    protected:

    private:
        bool directed; /*graf skierowany lub nie*/
        int **macierz; /*macierz sasiedztwa */
        int edge_num;
        int node_num;
        int neighbourCount = 0;
};

#endif // MATRIXGRAPH_H
