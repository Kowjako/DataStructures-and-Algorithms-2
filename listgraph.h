#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include <iostream>
#include <fstream>
#include "binaryheap.h"
#include <stack>
#include "matrixgraph.h"

using namespace std;

struct listEdge {
    int start;
    int finish;
    int weight;
};

struct node {
    int value;
    int weight;
    node* next;
};

class ListGraph
{
    public:
        ListGraph();
        ListGraph(int nodeNum, bool isDirected);

        bool connect(int startVertex, int endVertex, int weight);
        bool disconnect(int startVertex, int endVertex);

        void print();
        bool isDirected();
        void setDirected(bool isDirected);

        int getNodeNumber();
        int getEdgeNumber();

        void readFromFile(string filename);

        /** Algorytmy **/
        void dijkstraAlg(int start);
        void bellmanFordAlg(int start);
        void primAlg();
        void kruskalAlg();

        int getWeight(int start, int finish);
        listEdge** createSortedEdgesList();

        node** getHead();
        int* countNeighbours(int index);

        void clear();

        virtual ~ListGraph();

    protected:

    private:
        node **head;    /*tablica wskaznikow jako kolejnych wierzhoclkow oraz ich sasiadow */
        bool directed;
        int node_num;
        int edge_num;
        int neighbourCount;
        int** edgeMacierz;  /*lista krawedzi */
};

#endif // LISTGRAPH_H
