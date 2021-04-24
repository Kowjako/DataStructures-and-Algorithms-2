#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include <iostream>
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

        ListGraph& dijskstraAlg(int start);
        ListGraph& bellmanFordAlg(int start);

        void generateGraph(int n, int p, bool isDirected);
        bool readFromFile(string filename);

        void clear(int n, bool isDirected);
        int **getAdjMat();

        bool isDirected();
        void setDirected(bool isDirected);


    protected:

    private:
        bool directed;
        int **adj_mat{};
        int edge_num;
        int node_num;
};

#endif // MATRIXGRAPH_H
