#include "matrixgraph.h"

MatrixGraph::MatrixGraph()
{
    this->node_num = 0;
    this->directed = false;
}

MatrixGraph::~MatrixGraph()
{
    //dtor
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
    this->macierz = new int*[vertexNumber];
    for(int i=0;i<vertexNumber; i++) {
        this->macierz[i] = new int[vertexNumber];
        for(int j=0;j<vertexNumber;j++) {
            this->macierz[i][j] = 0;
        }
    }
}

bool MatrixGraph::connect(int startVertex, int endVertex, int edge)
{
    if(startVertex<0 || endVertex<0 || startVertex>node_num || endVertex>node_num) return false;

    this->edge_num++; /* Inkrementacja liczby krawedzi */
    this->macierz[startVertex][endVertex] = edge; /* Wpisywanie wartosci krawedzi do macierzy sasiedztwa */

    if(!this->directed) /* Jezeli graf nie jest skierowany wtedy od koncowego do poczatkowego taka sama krawedz */
        this->macierz[endVertex][startVertex] = edge;
    return true;
}

bool MatrixGraph::disconnect(int startVertex, int endVertex)
{
    if(startVertex<0 || endVertex<0 || startVertex>node_num || endVertex>node_num) return false;

    /* Usuwanie krawedzi poprzez wpisanie 0 */
    if(macierz[startVertex][endVertex]!=0)
        this->edge_num--;
    this->macierz[startVertex][endVertex] = 0;

    /* Jezeli graf nie jest skierowany istnieje taka sama krawedz z wierzcholka koncowego */
    if(!this->directed)
        this->macierz[endVertex][startVertex] = 0;
    return true;
}

void MatrixGraph::print() { /*Wypisanie macierzy sasiadztwa */
    for(int i=0;i<node_num;i++) {
        for(int j = 0;j<node_num;j++)
            cout<<this->macierz[i][j]<<" ";
        cout<<endl;
    }
}

int MatrixGraph::getNodeNumber() {
    return node_num;
}

int** MatrixGraph::getMacierz() {
    return macierz;
}

int MatrixGraph::getEdgeNumber() {
    return edge_num;
}

bool MatrixGraph::isDirected() {
    return directed;
}

void MatrixGraph::setDirected(bool tmpDirected) {
    this->directed = tmpDirected;
}

void MatrixGraph::clear(int nodeNumber, bool isDirected) {
    for(int i=0;i<this->node_num ;i++)
        delete[] this->macierz[i];
    delete[] this->macierz;

    this->node_num = nodeNumber;
    this->directed = isDirected;
    this->macierz = new int*[this->node_num];

    for(int i=0;i<nodeNumber;i++) {
        this->macierz[i] = new int[nodeNumber];
        for(int j=0;j<nodeNumber;j++)
            this->macierz[i][j] = 0;
    }
}

bool MatrixGraph::readFromFile(string filename) {
    ifstream file(filename);
    int edgeNum, nodeNum;
    file>>edgeNum; /*wczytanie liczby krawedzi */
    file>>nodeNum; /*wczytanie liczby wierzcholkow */

    string str;
    getline(file, str);

    clear(nodeNum, this->directed);
    this->node_num = nodeNum;

    this->macierz = new int*[nodeNum];
    for(int i = 0;i<nodeNum;i++) {
        this->macierz[i] = new int[nodeNum];
        for(int j = 0;j<nodeNum;j++)
            this->macierz[i][j] = 0;
    }

    int start, finish, weight;
    for(int i=0;i<edgeNum;i++) {
        file>>start;
        file>>finish;
        file>>weight;
        connect(start,finish, weight);
    }
}

ListGraph& MatrixGraph::dijskstraAlg(int start) {
    bool allVisited = false;
    int *length = new int[this->node_num];
    bool *visited = new bool[this->node_num];
    ListGraph *roads = new ListGraph(this->node_num,true);

    for(int i=0;i<this->node_num;i++) {
        visited[i] = false;
        length[i] = INT32_MAX;
        roads->connect(i, start, 0);
    }

    visited[start] = true;
    length[start] = 0;

    for(int i=0;i<this->node_num;i++) {
        if(this->macierz[i][start] != 0) {
            length[i] = macierz[i][start];
            roads->connect(i,i,macierz[i][start]);
        }
    }
    while(!allVisited) {
        int minLength = INT32_MAX;
        int order = -1;
        for(int i=0;i<this->node_num;i++) {
            if(length[i] <= minLength && !visited[i]) {
                order = i;
                minLength = length[i];
            }
        }
        node* currentRoad;
        visited[order] = true;

        for(int i=0;i<this->node_num;i++) {
            if(this->macierz[i][order]!=0) {
                int weight = macierz[i][order];
                if(!visited[i]) {
                    int currentDistance = length[order] + weight;
                    if(currentDistance < length[i]) {
                        length[i] = currentDistance;
                        roads->connect(order,i,weight);
                        currentRoad = roads->getHead()[order];
                        roads->setPath(currentRoad,i);
                        roads->disconnect(order,i);
                    }
                }
            }
        }

        allVisited = true;
        for(int i=0;i<this->node_num;i++) {
            if(!visited[i]) {
                allVisited = false;
                break;
            }
        }
    }
    return *roads;
}



