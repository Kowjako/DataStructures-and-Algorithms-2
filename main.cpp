#include <iostream>
#include "matrixgraph.h"
#include "listgraph.h"
#include "binaryheap.h"
#include <windows.h>
#include <iomanip>
using namespace std;

/** Inicjalizacja obiektow do pracy **/
MatrixGraph matrix;
ListGraph listg;

/** Odczyt czasu **/
long long int read_QPC()
{
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return((long long int)count.QuadPart);
}

/** Zmienne dla menu **/
bool isDirected;
string tmpLocation;
int operationNumber;

void workWithMatrixGraph() {
     do {
        cout<<"Aktualna reprezentacja: Macierz sasiedztwa"<<endl;
        cout<<"1.Wczytac graf z pliku"<<endl;
        cout<<"2.Algorytm Prima"<<endl;
        cout<<"3.Algorytm Kruskala"<<endl;
        cout<<"4.Algorytm Dijkstry"<<endl;
        cout<<"5.Algorytm Bellmana-Forda"<<endl;
        cout<<"6.Wyswietlic graf"<<endl;
        cout<<"7.Koniec programu"<<endl;
        cin>>operationNumber;
        switch(operationNumber) {
            case 1:
                cout<<"Wprowadz lokalizacje pliku np. D:/dane.txt"<<endl;
                cin>>tmpLocation;
                cout<<"Czy graf jest skierowany 1-tak, 0-nie"<<endl;
                cin>>isDirected;
                if(isDirected)
                    matrix.setDirected(true);
                else
                    matrix.setDirected(false);
                matrix.readFromFile(tmpLocation);
                break;
            case 2:
                if(isDirected) cout<<"Graf musi byc nieskierowanym"<<endl;
                else
                    matrix.primAlg();
                break;
            case 3:
                if(isDirected) cout<<"Graf musi byc nieskierowanym"<<endl;
                else
                    matrix.kruskalAlg();
                break;
            case 4:
                if(!isDirected) cout<<"Graf musi byc skierowanym"<<endl;
                else
                    matrix.dijkstraAlg(0);
                break;
            case 5:
                if(!isDirected) cout<<"Graf musi byc skierowanym"<<endl;
                else
                    matrix.bellmanFordAlg(0);
                break;
            case 6:
                if(matrix.getMacierz())
                    matrix.print();
                else cout<<"Nie wprowadzono grafu"<<endl;
                break;
            default:
                break;
        }
    } while(operationNumber!=7);
}

void workWithListGraph() {
     do {
        cout<<"Aktualna reprezentacja: Lista sasiadow"<<endl;
        cout<<"1.Wczytac graf z pliku"<<endl;
        cout<<"2.Algorytm Prima"<<endl;
        cout<<"3.Algorytm Kruskala"<<endl;
        cout<<"4.Algorytm Dijkstry"<<endl;
        cout<<"5.Algorytm Bellmana-Forda"<<endl;
        cout<<"6.Wyswietlic graf"<<endl;
        cout<<"7.Koniec programu"<<endl;
        cin>>operationNumber;
        switch(operationNumber) {
            case 1:
                cout<<"Wprowadz lokalizacje pliku np. D:/dane.txt"<<endl;
                cin>>tmpLocation;
                cout<<"Czy graf jest skierowany 1-tak, 0-nie"<<endl;
                cin>>isDirected;
                if(isDirected)
                    listg.setDirected(true);
                else
                    listg.setDirected(false);
                listg.readFromFile(tmpLocation);
                break;
            case 2:
                if(isDirected) cout<<"Graf musi byc nieskierowanym"<<endl;
                else
                    listg.primAlg();
                break;
            case 3:
                if(isDirected) cout<<"Graf musi byc nieskierowanym"<<endl;
                else
                    listg.kruskalAlg();
                break;
            case 4:
                if(!isDirected) cout<<"Graf musi byc skierowanym"<<endl;
                else
                    listg.dijkstraAlg(0);
                break;
            case 5:
                if(!isDirected) cout<<"Graf musi byc skierowanym"<<endl;
                else
                    listg.bellmanFordAlg(0);
                break;
            case 6:
                if(listg.getHead())
                    listg.print();
                else cout<<"Nie wprowadzono grafu"<<endl;
                break;
            default:
                break;
        }
    } while(operationNumber!=7);
}


int main()
{
    int structNumber;
    do {
        cout<<"Witam w Projekcie Nr.2"<<endl;
        cout<<"Prosze wybrac typ reprezentacji grafu"<<endl;
        cout<<"1. Lista sasiadow"<<endl;
        cout<<"2. Macierz sasiedztwa"<<endl;
        cout<<"3. Koniec programu"<<endl;
        cin>>structNumber;
        switch(structNumber) {
            case 1: workWithListGraph();
                    break;
            case 2: workWithMatrixGraph();
                    break;
            default:
                    break;
        }
    }
    while(structNumber!=3);

    return 0;
}

