#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include <iostream>
#include <fstream>

using namespace std;

struct edge {
    int start;
    int end;
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
        bool forceConnect(int startVertex, int endVertex, int cost);
        bool forceDisconnect(int startVertex, int endVertex);

        void add();
        void print();
        bool isDirected();
        void setDirected(bool isDirected);
        int getNodeNumber();
        int getEdgeNumber();
        bool isConnected(int startVertex, int endVertex, bool *visited);

        void setPath(node* start, int i);

        ListGraph& dijkstraAlg(int start);
        ListGraph& bellmanFordAlg(int start);

        node** getHead();
        int edgeWeight(node* startNode, node* finishNode);
        void copy(int **macierz, int n, bool isDirected);
        void clear();
        void printPaths(int start);

        virtual ~ListGraph();

    protected:

    private:
        node **head;
        bool directed;
        int node_num;
        int edge_num;
};

#endif // LISTGRAPH_H
