#include "listgraph.h"

ListGraph::~ListGraph()
{
    //dtor
}

ListGraph::ListGraph()
{
    this->head = nullptr;
    this->node_num = 0;
    this->edge_num = 0;
    this->directed = false;
}

ListGraph::ListGraph(int vertexNumber, bool isDirected) : node_num(vertexNumber), directed(isDirected)
{
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

bool ListGraph::connect(int start, int finish, int weight) {
    if(start<0 || finish<0 || start>=node_num || finish>=node_num) return false;

    node* newNode = new node;
    newNode->value = finish;
    newNode->weight = weight;
    newNode->next = nullptr;

    node* currentNode = head[start];
    node* prevNode = currentNode;

    if(head[start] == nullptr) {
        head[start] = newNode;
    }
    else
    {
        while(currentNode!=nullptr) {
            if(currentNode->value > newNode->value) {
                if(prevNode == currentNode) {
                    head[start] = newNode;
                    newNode->next = currentNode;
                }
                else {
                    prevNode->next = newNode;
                    newNode->next = currentNode;
                }
                break;
            }
            else if(currentNode->value == newNode->value) {
                this->edge_num++;
                return true;
            }
            prevNode = currentNode;
            currentNode = currentNode->next;
        }

        if(currentNode == nullptr)
            prevNode->next = newNode;
    }

    if(!this->directed) {
        return connect(finish,start,weight);
    }

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
        cout<<i;
        node* currentNode = head[i];    /*Bierzemy pierwszy element*/
        cout<<": ";
        while(currentNode!=nullptr) {   /* Poki nie jestesmy w koncu */
            cout<<currentNode->value<<"["<<currentNode->weight<<"]"<<" => "; /*Wypisujemy wierzcholek sasiada wraz z waga */
            currentNode = currentNode->next; /*Przechodzimy do kolejnego */
        }
        cout<<"null"<<endl;
    }
}

void ListGraph::add() {
    node** newHead;
    int nodeNumber = this->node_num++;
    newHead = new node*[nodeNumber];
    for(int i = 0 ;i<this->node_num;i++) {
        newHead[i] = head[i];
    }
    newHead[this->node_num] = nullptr;
    delete[] this->head;

    this->head = newHead;
    this->node_num++;
}

int ListGraph::edgeWeight(node* start, node* finish) {
    node* currentNode = this->head[start->value];
    while(currentNode!=nullptr) {
        if(currentNode->value == finish->value) {
            return currentNode->weight;
        }
        currentNode = currentNode->next;
    }
    return -1;  /*jezeli nie ma takiej krawedzi zwracamy -1 */
}

void ListGraph::setPath(node* start, int i) {
    delete this->head[i];
    this->head[i] = nullptr;
    node* currentNode = start;
    while(currentNode!=nullptr) {
        this->forceConnect(i, currentNode->value, currentNode->weight);
        currentNode = currentNode->next;
    }
}

void ListGraph::printPaths(int start) {
    if(head == nullptr) {
        cout<<"null"<<endl;
    }
    for(int i=0;i<this->node_num;i++) {
        cout<<"Path start "<<start<<" end "<<i;
        node* currentNode = head[i];
        cout<<": ";
        int tmp = 0;
        while(currentNode!=nullptr) {
            cout<<currentNode->value<<"["<<currentNode->weight<<"]"<<" => ";
            currentNode = currentNode->next;
            tmp++;
        }
        cout<<"null"<<endl;
    }
}

bool ListGraph::forceConnect(int start, int finish, int weight) {
    if(start<0 || finish<0 || start>=node_num || finish>=node_num) return false;

    node* newNode = new node;
    newNode->value = finish;
    newNode->weight = weight;
    newNode->next = nullptr;

    node* currentNode = head[start];
    node* prevNode = currentNode;

    if(currentNode!=nullptr) {
        currentNode = currentNode->next;
        while(currentNode!=nullptr) {
            currentNode =currentNode->next;
            prevNode = prevNode->next;
        }
        prevNode->next = newNode;
    }
    else {
        this->head[start] = newNode;
    }
    this->edge_num++;
    return true;
}

bool ListGraph::forceDisconnect(int start, int finish) {
    if(start<0 || finish<0 || start>=node_num || finish>=node_num) return false;

    node* currentNode = head[start];
    node* prevNode = currentNode;

    while(currentNode!=nullptr) {
        if(currentNode->value == finish) {
            prevNode->next = currentNode->next;
            delete currentNode;
            return true;
        }
        if(currentNode!=prevNode)
            prevNode = prevNode->next;
        currentNode = currentNode->next;
    }
    return false;
}