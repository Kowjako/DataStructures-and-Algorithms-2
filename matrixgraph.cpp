#include "matrixgraph.h"
#include "binaryheap.h"
MatrixGraph::MatrixGraph() {
    this->node_num = 0;
    this->directed = false;
}

MatrixGraph::~MatrixGraph() {
    clear(0, false);
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

    for(int i=0;i<this->node_num ;i++)
        delete[] this->edgeMacierz[i];
    delete[] this->edgeMacierz;

    /* Inicjalizacja nowego */
    this->node_num = nodeNumber;
    this->edge_num = 0;
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
    if(file.is_open()) {
        int edgeNum, nodeNum;
        file>>edgeNum; /*wczytanie liczby krawedzi */
        file>>nodeNum; /*wczytanie liczby wierzcholkow */

        if(file.fail()) {
            cout<<"Blad podczas odczytywania poczatkowych wartosci"<<endl;
            return false;
        }

        string str;
        getline(file, str);

        clear(nodeNum, this->directed);

        this->edgeMacierz = new int*[edgeNum]; /*tworzenie listy krawedzi potrzebnej do algroytmow */

        int start, finish, weight;
        for(int i=0;i<edgeNum;i++) {
            file>>start;
            file>>finish;
            file>>weight;
            if(file.fail()) cout<<"Blad podczas odczytywania krawedzi"<<endl;
            connect(start,finish, weight);
            this->edgeMacierz[i] = new int[2]; /*tworzymy miejsce na jedna krawedz */
            this->edgeMacierz[i][0] = start;
            this->edgeMacierz[i][1] = finish;
        }
    /** Wypisanie listy krawedzi - Opcjonalnie **/
    /* for(int i=0;i<getEdgeNumber();i++) {
        cout<<"("<<this->edgeMacierz[i][0]<<", "<<this->edgeMacierz[i][1]<<")";
        cout<<endl;
    } */
    }
    else cout<<"Blad podczas otwierania pliku"<<endl;
    return true;
}

void MatrixGraph::dijkstraAlg(int start) {
    BinaryHeap bheap; /*kolejka*/
    int u = 0;  /*aktualnie sprawdzany wierzcholek */

    int* edge = new int[2];
    bool* validated = new bool[this->node_num]; /*tablica sprawdzonych wierzcholkow*/
    int* d = new int[this->node_num];  /*tablica aktualnych odleglosci*/
    int* p = new int[this->node_num];  /*tablica koncowych wierzcholkow*/
    int* neighbours = nullptr; /* zbior sasiadow wierzcholka */

    /* Poczatkowa inicjalizacja */
    for(int i=0;i<this->node_num;i++) {
        d[i] = INT_MAX;
        p[i] = INT_MAX;
        validated[i] = false;
    }
    d[start] = 0;

    /*Inicjalizacja kolejki priorytetowej (kopiec-min) */
    for(int i=0;i<this->node_num;i++)
        bheap.addItem(d[i]);

    /* Sprawdzenie wag ujemnych */
    for(int i=0;i<this->edge_num;i++) {
        edge = this->edgeMacierz[i];
        if(getWeight(edge[0],edge[1])<0) {
            cout<<"Istnieja wagi ujemne, algorytm Dijkstry nie moze byc wykonany"<<endl;
            return;
        }
    }

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

    /** Wyswietlenie wyniku z wykorzystaniem stosu **/
    stack<int> roads;
    cout<<"Start = "<<start<<endl;
    for(int i = 1;i<this->node_num;i++) {
        cout<<"To: "<<i<<" Dist: "<<d[i]<<" Path: ";
        int address = i;
        roads.push(i);
        while(p[address]!=INT_MAX) {
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
    delete edge;
}

void MatrixGraph::bellmanFordAlg(int start) {
    int counter = 0;
    int* edge = new int[2];
    int* d = new int[this->node_num];  /*tablica aktualnych odleglosci*/
    int* p = new int[this->node_num];  /*tablica koncowych wierzcholkow*/
    for(int i=0;i<this->node_num;i++) {
        d[i] = INT_MAX-100;
        p[i] = INT_MAX;
    }
    d[start] = 0;

    for(int i=0;i<this->node_num-1;i++) {
        for(int j=0;j<getEdgeNumber();j++) {
            edge = this->edgeMacierz[counter];
            if(d[edge[1]] > d[edge[0]] + getWeight(edge[0],edge[1])) {
                d[edge[1]] = d[edge[0]] + getWeight(edge[0],edge[1]);
                if(edge[1]==0) continue;
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
        while(p[address]!=INT_MAX) {
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

void MatrixGraph::primAlg() {
    bool* validated  = new bool[this->node_num];
    int* neighbours = nullptr;
    int* key = new int[this->node_num];
    int* p = new int[this->node_num];
    BinaryHeap bHeap;

    for(int i=0;i<this->node_num;i++) {
        key[i] = INT_MAX;
        validated[i] = false;
    }
    key[0] = 0;
    p[0] = INT_MAX;

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

        for(int i = 0; i<this->neighbourCount;i++) { /*Sprawdzamy droge do kazdego */
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

    /*Zwolnienie pamieci */
    delete[] validated;
    delete[] neighbours;
    delete[] key;
    delete[] p;
}

void MatrixGraph::kruskalAlg() {
    bool* isEdgeMakeSolution = new bool[this->edge_num];
    for(int i=0;i<this->edge_num;i++) {
        isEdgeMakeSolution[i] = false;
    }
    int* grupa = new int[this->node_num];
    edge* actualEdge = nullptr;

    /*Tworzenie posortowanej listy krawedzi*/
    edge** edges = createSortedEdgesList();
    /* Tworzenie poddrzew */
    for(int i=0;i<this->node_num;i++) {
        grupa[i] = i;
    }
    for(int i = 0;i<this->edge_num;i++) {
        actualEdge = edges[i];
        if(grupa[actualEdge->start] != grupa[actualEdge->finish]) {
            isEdgeMakeSolution[i] = true;
            int changeValue = grupa[actualEdge->finish]; /* kopia indeksu grupy do zmiany zeby nie byla utracona po pierwszej podmianie */
            for(int i=0;i<this->node_num;i++) {
                if(grupa[i] == changeValue)
                    grupa[i] = grupa[actualEdge->start];
            }
        }
    }
    /*Wyswietlenie wyniku*/
    int sum = 0;
    for(int i=0;i<this->edge_num;i++) {
        if(isEdgeMakeSolution[i]) {
            cout<<"("<<edges[i]->start<<";"<<edges[i]->finish<<")"<<" -> "<<edges[i]->weight<<endl;
            sum+=edges[i]->weight;
        }
    }
    cout<<"MST = "<<sum<<endl;
    /*Zwolnienie pamieci*/
    delete[] isEdgeMakeSolution;
    delete[] grupa;
    delete[] edges;
    delete actualEdge;
}

edge** MatrixGraph::createSortedEdgesList() {
    edge** unsortedEdges = new edge*[getEdgeNumber()];
    /*Tworzenie nieposortowanej listy krawedzi wraz z wagami*/
    for(int i=0;i<this->edge_num;i++) {
        unsortedEdges[i] = new edge;
        unsortedEdges[i]->start = this->edgeMacierz[i][0];
        unsortedEdges[i]->finish = this->edgeMacierz[i][1];
        unsortedEdges[i]->weight = this->macierz[this->edgeMacierz[i][0]][this->edgeMacierz[i][1]];
    }

    /* Sortowanie selekcyjne wedlug wag */
    edge* temp = nullptr;
    int SIZE = this->getEdgeNumber();
    for(int i=0;i<SIZE-1;i++) {
        edge* maximum = unsortedEdges[0];
        int indmax = 0;
        for(int j=0;j<SIZE-i;j++) {
            if(unsortedEdges[j]->weight>maximum->weight) {
                maximum=unsortedEdges[j];
                indmax = j;
            }
        }
        temp = unsortedEdges[SIZE-1-i];
        unsortedEdges[SIZE-1-i] = maximum;
        unsortedEdges[indmax] = temp;
    }

    /** Wypisanie listy krawedzi wraz z wagami po sortowaniu- Opcjonalnie**/
    /* for(int i=0;i<getEdgeNumber();i++) {
        cout<<"("<<unsortedEdges[i]->start<<", "<<unsortedEdges[i]->finish<<")"<<":"<<unsortedEdges[i]->weight;
        cout<<endl;
    } */
    return unsortedEdges;
}

void MatrixGraph::createRandomGraph(int vertexNumber, bool isDirected, int edgeNumber) {
    /* Usuniecie poprzedniego gdy istnial i inicjowanie nowego */
    clear(vertexNumber,isDirected);

    /*Tworzenie listy krawedzi dla alg. Kruskala */
    this->edgeMacierz = new int*[edgeNumber];

    /* Maks. ilosc krawedzi to graf pelny wynosi n*(n-1)/2 a minimalna to n-1*/
    if(edgeNumber>(vertexNumber)*(vertexNumber-1)*0.5 || edgeNumber<vertexNumber-1 && !isDirected) {
        cout<<"Nie moze byc tyle krawedzi!"<<endl;
        return;
    }

    int maxEdgeNumber = (vertexNumber * vertexNumber - vertexNumber) / 2; /* graf pelny */
    int edgeCounter = 0;

    /* Tworzenie grafu minimalnego = spojnego */
    for (int i=0; i<this->node_num-1;i++) { /* minimalna ilosc krawedzi = n - 1 aby graf byl spojny */
        connect(i,i + 1,rand()%(INT_MAX-200));
        this->edgeMacierz[edgeCounter] = new int[2]; /*tworzymy miejsce na jedna krawedz */
        this->edgeMacierz[edgeCounter][0] = i;
        this->edgeMacierz[edgeCounter][1] = i+1;
        edgeCounter++;
    }

    int i,j;
    /*Nastepnie tworzymy aby osiagnac zadana ilosc krawedzi */
    while(this->edge_num!=edgeNumber) {
        i = rand()%vertexNumber;
        j = rand()%vertexNumber;
        if(this->macierz[i][j]==0 && i!=j) {    /*jezeli nie ma polaczenia oraz wierzcholek nie wskazuje na siebie*/
            connect(i,j,rand()%(INT_MAX-200));   /*polacz*/
            this->edgeMacierz[edgeCounter] = new int[2]; /*tworzymy miejsce na jedna krawedz */
            this->edgeMacierz[edgeCounter][0] = i;
            this->edgeMacierz[edgeCounter][1] = j;
            edgeCounter++;
        }
    }
}
