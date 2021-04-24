#include "matrixgraph.h"
#include "binaryheap.h"
MatrixGraph::MatrixGraph()
{
    this->node_num = 0;
    this->directed = false;
}

MatrixGraph::~MatrixGraph()
{
    for(int i=0;i<this->node_num ;i++)
        delete[] this->macierz[i];
    delete[] this->macierz;
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
    /*Usuniecie poprzedniego*/
    for(int i=0;i<this->node_num ;i++)
        delete[] this->macierz[i];
    delete[] this->macierz;

    /* Inicjalizacja nowego */
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

    int start, finish, weight;
    for(int i=0;i<edgeNum;i++) {
        file>>start;
        file>>finish;
        file>>weight;
        connect(start,finish, weight);
    }
}

void MatrixGraph::dijkstraAlg(int start) {
    BinaryHeap bheap; /*kolejka*/
    int u = 0;
    bool* validated = new bool[this->node_num]; /*tablica sprawdzonych wierzcholkow*/
    int* d = new int[this->node_num];  /*tablica aktualnych odleglosci*/
    int* p = new int[this->node_num];  /*tablica koncowych wierzcholkow*/
    int* neighbours; /* zbior sasiadow wierzcholka */
    for(int i=0;i<this->node_num;i++) {
        d[i] = 10000;
        p[i] = 10000;
        validated[i] = false;
    }
    d[0] = 0;

    /*Inicjalizacja kolejki priorytetowej (kopiec-min) */
    for(int i=0;i<this->node_num;i++)
        bheap.addItem(d[i]);
    while(bheap.sizeVar!=0) {
        int u;
        int x = bheap.deleteVertex(); /*Pobieramy najmniejsza wage*/
        /*Znalezenie numeru wierzcholka o najmniejszej wadze d[i]*/
        for(int i=0;i<this->node_num;i++) {
            if(d[i] == x && validated[i] == false)
            u = i;
        }

        neighbours = countNeighbours(u); /*szukamy liste indeksow sasiadow */
        for(int i = 0; i<this->neighbourCount;i++) { /*Sprwadzamy droge do kazdego */
            int v = neighbours[i];
            if (d[v] > d[u] + getWeight(u,v)) {
                d[v] = d[u] + getWeight(u,v); /* Relaksacja */
                p[v] = u;
            }
        }

        /*Oznaczamy wierzcholek jako sprawdzony */
        validated[u] = true;
        bheap.deleteHeap();

        /*Tworzymy kolejke na podstawie korektowanych drog */
        for(int i=0;i<this->node_num;i++) {
            if(validated[i]!=true)
                bheap.addItem(d[i]);
        }
    }
    /* Wyswietlenie wyniku */
    cout<<"Start = "<<start<<endl;
    for(int i = 1;i<this->node_num;i++) {
        cout<<"To: "<<i<<" Dist: "<<d[i]<<" Path: ";
        int address = i;
        cout<<i<<"<-";
        while(p[address]!=10000) {
            if(p[address]==0)
                cout<<p[address];
            else
                cout<<p[address]<<"<-";
            address = p[address];
        }
        cout<<endl;
    }

    /* Zwolnienie pamieci */
    delete[] validated;
    delete[] p;
    delete[] d;
    delete[] neighbours;
}

int* MatrixGraph::countNeighbours(int index) {
    int neighbours = 0;
    /* Obliczamy ilosc sasiadow */
    for(int j = 0;j<node_num;j++) {
        if(this->macierz[index][j]!=0)
            neighbours++;
    }
    /*Ustwaiamy ile ma sasiadow*/
    this->neighbourCount = neighbours;

    int* neighbourArray = new int[neighbours];
    int counter = 0;
    for(int j = 0;j<node_num;j++) {
        if(this->macierz[index][j]!=0) {
            neighbourArray[counter] = j;
            counter++;
        }
    }
    return neighbourArray;
}

int MatrixGraph::getWeight(int start, int finish) {
    return this->macierz[start][finish];
}



