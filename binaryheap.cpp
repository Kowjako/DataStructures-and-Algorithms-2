#include "binaryheap.h"

BinaryHeap::BinaryHeap()
{
    //ctor
}
BinaryHeap::~BinaryHeap()
{
    sizeVar = 0;
    free(heap);
    heap = NULL;
}

void BinaryHeap::showBinaryHeap() {
    int levels = log2(sizeVar)+1;
    int i = 0;
    int j = 1;
    while(i < sizeVar) {
        while((i < j) && (i < sizeVar)) {
            for(int i = 0;i<levels*2;i++)
                cout<<" ";
            cout << heap[i] << " ";
            for(int i = 0;i<levels;i++)
                cout<<" ";
            i++;
        }
        levels = levels/2;
        cout << endl;
        j = j * 2 + 1;
    }
}

void BinaryHeap::readFromFile(string filename) {
    int temporaryInt = 0, temporarySize = 0;
    ifstream in(filename);
    if(in.is_open()) {
        in>>temporarySize;
        if(in.fail()) cout<<"Blad podczas odczytywania rozmiaru"<<endl;
        else {
            for(int i=0;i<temporarySize;i++) {
                in>>temporaryInt;
                if(in.fail()) cout<<"Blad odczytywania wartosci"<<endl;
                else
                addItem(temporaryInt);
            }
            in.close();
        }
    }
    else cout<<"Blad przy otwieraniu pliku"<<endl;
}

void BinaryHeap::addItem(int item) {
    int* tmp = (int*)calloc((sizeVar+1),sizeof(int));
    for(int i=0;i<sizeVar;i++) {
        tmp[i]= heap[i];
    }
    free(heap);
    heap = tmp;
    addToNormalizedPosition(item);
}

int BinaryHeap::deleteVertex() {
    int deletedValue = heap[0];
    if(sizeVar == 0) return -1;
    heap[0] = heap[sizeVar-1]; /*usuniecie szczytu jako zamiana na ostatni element */
    heap = (int*)realloc(heap, sizeof(int)*(sizeVar-1));
    sizeVar--;
    normalize(0); /* usuniecie szczytu wymaga normalizacji od korzenia */
    return deletedValue;
}

void BinaryHeap::normalize(int index) {
    int temp;
    int leftItem = 2*index+1;
    int rightItem = 2*index+2;

    if(rightItem<sizeVar) { /*sprawdzamy czy istnieje prawy potomek */
        if(heap[rightItem]<heap[leftItem] && heap[rightItem]<heap[index]) { /*sprawdzamy czy jest mneisjzy od lewego oraz rodzica */
            temp = heap[index];
            heap[index] = heap[rightItem];
            heap[rightItem] = temp;
            normalize(rightItem);
        }
        else {
            if(heap[leftItem]<heap[index]) { /*sprawdzamy czy lewy jest mneijszy niz rodzic */
                temp = heap[index];
                heap[index] = heap[leftItem];
                heap[leftItem] = temp;
                normalize(leftItem);
            }
        }
    }
    else
        if(leftItem<sizeVar) { /*tu trafimy gdy nie ma prawego potomka*/
            if(heap[leftItem]<heap[index]) { /*sprawdzamy czy jest mniejszy od rodzica */
                temp = heap[index];
                heap[index] = heap[leftItem];
                heap[leftItem] = temp;
                normalize(leftItem);
            }
        }
        else return;
}

void BinaryHeap::initializate(int sizeValue, int value) {
    heap = (int*)calloc(sizeValue,sizeof(int));
    for(int i=0;i<sizeValue;i++) {
        addToNormalizedPosition(value);
    }
}

bool BinaryHeap::findElement(int i,int item) { /* wywolanie funkcji gdzie i wstawiamy 0 - poczatek */
    if(item>heap[i]) return false;
    if(item==heap[i]) {
        return true;
    }

    int left = 2*i+1;
    if(left>=sizeVar)
        return false; /* jezeli nie ma lewego to prawego nie moze byc */

    bool findLeft = findElement(left, item);
    if(findLeft!=false) {
        return true;
    }

    int right = 2*i+2;
    if(right>=sizeVar)
        return false;
    else
        return findElement(right,item);

}

void BinaryHeap::addToNormalizedPosition(int element) {
    int parentNode;
    int i = sizeVar;
    heap[sizeVar] = element; /* dodanie nowego elementu na koniec tablicy */
    parentNode = (i-1)/2; /* indeks rodzica  */
    while(i>0) {
        if(heap[i]<heap[parentNode]) { /* jezeli jest mniejszy niz rodzic robimy swap */
            int tmp = heap[i];
            heap[i] = heap[parentNode];
            heap[parentNode] = tmp;
        }
        i = parentNode;   /* ustawiamy pozycje elementu jako pozycje rodzica */
        parentNode = (i-1)/2; /* obliczamy pozycje "nowego" rodzica */
    }
    sizeVar++;
}

void BinaryHeap::deleteHeap() {
    while(sizeVar!=0) {
        deleteVertex();
    }
}
