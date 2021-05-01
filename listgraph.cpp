#include "listgraph.h"

ListGraph::~ListGraph() {
    //dtor
}

ListGraph::ListGraph() {
    this->head = nullptr;
    this->node_num = 0;
    this->edge_num = 0;
    this->directed = false;
}

ListGraph::ListGraph(int vertexNumber, bool isDirected) : node_num(vertexNumber), directed(isDirected) {
    this->edge_num = 0;
    this->head = new node*[vertexNumber];
    for(int i=0;i<this->node_num;i++) {
        head[i] = nullptr;
    }
}

bool ListGraph::isDirected() {
    return this->directed;
}

void ListGraph::setDirected(bool isDirected) {
    this->directed = isDirected;
}

int ListGraph::getEdgeNumber() {
    return this->edge_num;
}

int ListGraph::getNodeNumber() {
    return this->node_num;
}

node** ListGraph::getHead() {
    return head;
}

void ListGraph::clear() {
    for (int i = 0; i < this->node_num; i++) {
        delete this->head[i];
        this->head[i] = nullptr;
    }
    delete[] this->head;
    this->head = nullptr;
    this->node_num = 0;
    this->edge_num = 0;
}

void ListGraph::readFromFile(string filename) {
    ifstream file(filename);
    int edgeNum, nodeNum;
    file>>edgeNum; /*wczytanie liczby krawedzi */
    file>>nodeNum; /*wczytanie liczby wierzcholkow */

    string str;
    getline(file, str);

    this->edgeMacierz = new int*[edgeNum];

    this->node_num = nodeNum;
    this->head = new node*[node_num];
    for(int i=0;i<this->node_num;i++) {
        head[i] = nullptr;
    }

    int start, finish, weight;
    for(int i=0;i<edgeNum;i++) {
        file>>start;
        file>>finish;
        file>>weight;
        connect(start,finish, weight);
        this->edgeMacierz[i] = new int[2]; /*tworzymy miejsce na jedna krawedz */
        this->edgeMacierz[i][0] = start;
        this->edgeMacierz[i][1] = finish;
    }
}

bool ListGraph::connect(int start, int finish, int weight) {
    if(start<0 || finish<0 || start>=node_num || finish>=node_num) return false;

    /* Tworzymy nowy wezel */
    node* newNode = new node;
    newNode->value = finish;
    newNode->weight = weight;
    newNode->next = nullptr;

    node* currentNode = head[start];
    node* prevNode = currentNode;

    /*Jezeli start nie ma sasiadow to robimy jako pierwszego */
    if(head[start] == nullptr) {
        head[start] = newNode;
    }
    else    /*Jezeli ma sasiadow dodajemy na koniec */
    {
        while(currentNode!=nullptr) {
            /*Wstawianie w posortowanej kolejnosci */
            if(currentNode->value > newNode->value) {   /*Sprawdzamy czy obecny indeks jest wiekszy */
                if(prevNode == currentNode) {
                    head[start] = newNode;      /*Jezeli wiekszy to wstawiamy nasz nowy*/
                    newNode->next = currentNode;    /*Przesuwamy obecny jako nastepny odnosnie wstawionego */
                }
                else {
                    prevNode->next = newNode;
                    newNode->next = currentNode;
                }
                break;
            }
            else
            if(currentNode->value == newNode->value) {
                this->edge_num++;
                return true;
            }
            /* Przechodzimy do kolejnego sasiada */
            prevNode = currentNode;
            currentNode = currentNode->next;
        }

        if(currentNode == nullptr)
            prevNode->next = newNode;
    }

    /*Jezeli skierowany to laczymy odwrotnie tez */
    if(!this->directed) {
        return connect(finish,start,weight);
    }

    this->edge_num++;
    return true;
}

bool ListGraph::disconnect(int start, int finish) {
    if(start<0 || finish<0 || start>=node_num || finish>=node_num) return false;

    node* currentNode = head[start];
    node* prevNode = currentNode;

    if(head[start] == nullptr) {
        return true;
    }
    else {
        while(currentNode!=nullptr) {
            if(currentNode->value > finish) {
                this->edge_num--;
                return true;
            }
            else if(currentNode->value == finish) {
                if(currentNode->next!=nullptr) {
                    if(prevNode!= currentNode) {
                        prevNode->next = currentNode->next;
                    }
                    else {
                        head[start] = currentNode->next;
                    }
                } else {
                    if(prevNode->next != nullptr) {
                        prevNode->next = nullptr;
                    }
                    else {
                        head[start] = nullptr;
                    }
                }
                currentNode = nullptr;
                delete currentNode;
                break;
            }

            prevNode = currentNode;
            currentNode = currentNode->next;
        }
    }
    /*Jezeli skierowany to usuwamy tez odwrotnie */
    if(!this->directed) {
        return disconnect(finish,start);
    }

    return true;
}

void ListGraph::print() {
    if(head==nullptr) {
        cout<<"puste"<<endl;
    }
    for(int i = 0;i<node_num;i++) {
        cout<<i;    /*Indeks wierzcholku */
        node* currentNode = head[i];    /*Bierzemy pierwszy element*/
        cout<<": ";
        while(currentNode!=nullptr) {   /* Poki nie jestesmy w koncu */
            cout<<currentNode->value<<"["<<currentNode->weight<<"]"<<" => "; /*Wypisujemy wierzcholek sasiada wraz z waga */
            currentNode = currentNode->next; /*Przechodzimy do kolejnego */
        }
        cout<<"null"<<endl;
    }
}

void ListGraph::dijkstraAlg(int start) {
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

int* ListGraph::countNeighbours(int index) {
    int neigboursCount = 0;
    node* start = head[index];
    /*Obliczamy ile ma sasiadow */
    while(start!=nullptr) {
        neigboursCount++;
        start = start->next;
    }
    /*Ustawiamy ilosc sasiadow */
    this->neighbourCount = neigboursCount;

    /*Tworzymy liste indeksow sasiadow */
    int* neigbourArray = new int[neigboursCount];
    /*Zwracamy wskaznik na poczatek */
    start = head[index];
    for(int i=0;i<neigboursCount;i++) {
        neigbourArray[i] = start->value;
        start = start->next;
    }
    return neigbourArray;
}

int ListGraph::getWeight(int start, int finish) {
    /*Przechodzimy do potrzebnej listy */
    node* startNode = head[start];
    while(startNode!=nullptr) {
        if(startNode->value==finish)
            return startNode->weight;
        startNode = startNode->next;
    }
}

void ListGraph::bellmanFordAlg(int start) {
    cout<<"edge num = "<<getEdgeNumber()<<endl;
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
        /*Odkladamy na stosie az do poczatkowego bo poprzednik poczatkowego to 10000*/
        while(p[address]!=10000) {
            roads.push(p[address]);
            address = p[address];
        }
        /*Pobieramy ze stosu i wyswietlamy */
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

void ListGraph::kruskalAlg() {
}
