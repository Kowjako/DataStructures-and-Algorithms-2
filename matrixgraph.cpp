#include "matrixgraph.h"

MatrixGraph::MatrixGraph()
{
    this->node_num = 0;
    this->directed = false;
}

/*Inicjalizacja zmiennych prywatnych */
MatrixGraph::MatrixGraph(int vertexNumber, bool isDirected) : node_num(vertexNumber), directed(isDirected)
{
    this->edge_num = 0;
    if(vertexNumber < 0) {
        this->node_num = 0;
    }
    else {
        this->node_num = vertexNumber;
    }

    /*Tworzenie poczatkowej macierzy sasiedztwa */
    this->adj_mat = new int*[vertexNumber];
    for(int i=0;i<vertexNumber; i++) {
        this->adj_mat[i] = new int[vert];
        for(int j=0;j<vert;j++) {
            this->adj_mat[i][j] = 0;
        }
    }
}

MatrixGraph::~MatrixGraph()
{
    //dtor
}
