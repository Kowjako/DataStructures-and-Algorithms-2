#include "matrixgraph.h"
#include "binaryheap.h"
MatrixGraph::MatrixGraph() {
    this->node_num = 0;
    this->directed = false;
}

MatrixGraph::~MatrixGraph() {
    for(int i=0;i<this->node_num ;i++)
        delete[] this->macierz[i];
    delete[] this->macierz;
}

MatrixGraph::MatrixGraph(int vertexNumber, bool isDirected) : node_num(vertexNumber), directed(isDirected) {
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

int* MatrixGraph::countNeighbours(int index) {
    int neighbours = 0;
    /* Obliczamy ilosc sasiadow */
    for(int j = 0;j<node_num;j++) {
        if(this->macierz[index][j]!=0)
            neighbours++;
    }
    /*Ustwaiamy ile ma sasiadow*/
    this->neighbourCount = neighbours;

    /* Tworzymy liste indeksow sasiadow */
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

bool MatrixGraph::connect(int startVertex, int endVertex, int edge) {
    if(startVertex<0 || endVertex<0 || startVertex>node_num || endVertex>node_num) return false;

    this->edge_num++; /* Inkrementacja liczby krawedzi */
    this->macierz[startVertex][endVertex] = edge; /* Wpisywanie wartosci krawedzi do macierzy sasiedztwa */

    if(!this->directed) /* Jezeli graf nie jest skierowany wtedy od koncowego do poczatkowego taka sama krawedz */
        this->macierz[endVertex][startVertex] = edge;
    return true;
}

bool MatrixGraph::disconnect(int startVertex, int endVertex) {
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
    int u = 0;  /*aktualnie sprawdzany wierzcholek */

    bool* validated = new bool[this->node_num]; /*tablica sprawdzonych wierzcholkow*/
    int* d = new int[this->node_num];  /*tablica aktualnych odleglosci*/
    int* p = new int[this->node_num];  /*tablica koncowych wierzcholkow*/
    int* neighbours = nullptr; /* zbior sasiadow wierzcholka */

    /* Poczatkowa inicjaliacja */
    for(int i=0;i<this->node_num;i++) {
        d[i] = 10000;
        p[i] = 10000;
        validated[i] = false;
    }
    d[start] = 0;

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
        /*szukamy liste indeksow sasiadow */
        neighbours = countNeighbours(u);
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

    /* Wyswietlenie wyniku z wykorzystaniem stosu*/
    stack<int> roads;
    cout<<"Start = "<<start<<endl;
    for(int i = 1;i<this->node_num;i++) {
        cout<<"To: "<<i<<" Dist: "<<d[i]<<" Path: ";
        int address = i;
        roads.push(i);
        while(p[address]!=10000) {
            roads.push(p[address]);
            address = p[address];
        }
        while(!roads.empty()) {
            if(roads.size()==1)
                cout<<roads.top()<<"";
            else
                cout<<roads.top()<<"->";
            roads.pop();
        }
        cout<<endl;
    }
    /* Zwolnienie pamieci */
    delete[] validated;
    delete[] p;
    delete[] d;
    delete[] neighbours;
}

void MatrixGraph::bellmanFordAlg(int start) {
    createListOfEdges();
    int counter = 0;
    int* edge = new int[2];
    int* d = new int[this->node_num];  /*tablica aktualnych odleglosci*/
    int* p = new int[this->node_num];  /*tablica koncowych wierzcholkow*/
    for(int i=0;i<this->node_num;i++) {
        d[i] = 10000;
        p[i] = 10000;
    }
    d[start] = 0;

    for(int i=0;i<this->node_num-1;i++) {
        for(int j=0;j<getEdgeNumber();j++) {
            edge = this->edgeMacierz[counter];
            if(d[edge[1]] > d[edge[0]] + getWeight(edge[0],edge[1])) {
                d[edge[1]] = d[edge[0]] + getWeight(edge[0],edge[1]);
                p[edge[1]] = edge[0];
            }
            counter++;
        }
        counter = 0; /*bo trzeba znow przejsc przez kazda krawedz*/
    }
    for(int i=0;i<this->edge_num;i++) {
        edge = this->edgeMacierz[i];
        if(d[edge[1]] > d[edge[0]] + getWeight(edge[0],edge[1])) {
            cout<<"Cykl ujemny w algorytmie Bellmana-Forda"<<endl;
            return;
        }
    }

    /*Wyswietlanie wyniku*/
    stack<int> roads;
    cout<<"Start = "<<start<<endl;
    for(int i = 1;i<this->node_num;i++) {
        cout<<"To: "<<i<<" Dist: "<<d[i]<<" Path: ";
        int address = i;
        roads.push(i);
        while(p[address]!=10000) {
            roads.push(p[address]);
            address = p[address];
        }
        while(!roads.empty()) {
            if(roads.size()==1)
                cout<<roads.top()<<"";
            else
                cout<<roads.top()<<"->";
            roads.pop();
        }
        cout<<endl;
    }
    /*Zwolnienie pamieci */
    delete edge;
    delete[] d;
    delete[] p;
}

void MatrixGraph::createListOfEdges() {
    int counter = 0;
    this->edgeMacierz = new int*[getEdgeNumber()];
    for(int j=0;j<node_num;j++) {
        for(int k=0;k<node_num;k++)
        if(this->macierz[j][k]!=0) {
            this->edgeMacierz[counter] = new int[2]; /*tworzymy miejsce na jedna krawedz */
            this->edgeMacierz[counter][0] = j;
            this->edgeMacierz[counter][1] = k;
            counter++;
        }
    }

    /** Wypisanie listy krawedzi **/
    /* for(int i=0;i<getEdgeNumber();i++) {
        cout<<"("<<this->edgeMacierz[i][0]<<", "<<this->edgeMacierz[i][1]<<")";
        cout<<endl;
    } */
}

void MatrixGraph::primAlg(int start) {
    bool* validated  = new bool[this->node_num];
    int* neighbours = nullptr;
    int* key = new int[this->node_num];
    int* p = new int[this->node_num];
    BinaryHeap bHeap;

    for(int i=0;i<this->node_num;i++) {
        key[i] = 10000;
        validated[i] = false;
    }
    key[start] = 0;
    p[start] = 10000;

    /*Inicjalizacja kolejki priorytetowej*/
    for(int i=0;i<this->node_num;i++)
        bHeap.addItem(key[i]);

    while(bHeap.sizeVar!=0) {
        int u = 0;
        int x = bHeap.deleteVertex();
        /*Znalezenie numeru wierzcholka o najmniejszej wadze d[i]*/
        for(int i=0;i<this->node_num;i++) {
            if(key[i] == x && validated[i] == false)
            u = i;
        }

        neighbours = countNeighbours(u);

        for(int i = 0; i<this->neighbourCount;i++) { /*Sprwadzamy droge do kazdego */
            int v = neighbours[i];
            if(bHeap.sizeVar!=0 && bHeap.findElement(0,key[v]) && getWeight(u,v) < key[v]) {
                key[v] = getWeight(u,v);
                p[v] = u;
            }
        }
        validated[u] = true;
        bHeap.deleteHeap();

        for(int i=0;i<this->node_num;i++) {
            if(validated[i]!=true)
                bHeap.addItem(key[i]);
        }
    }

    /*Koncowe wyswietlanie*/
    int sum = 0;
    for(int i=1;i<this->node_num;i++) {
        cout<<"("<<p[i]<<";"<<i<<")"<<" -> "<<key[i]<<endl;
        sum+=key[i];
    }
    cout<<"MST = "<<sum<<endl;
}

void MatrixGraph::kruskalAlg(int start) {

}



