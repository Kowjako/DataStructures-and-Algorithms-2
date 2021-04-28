#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
using namespace std;

class BinaryHeap
{
    public:
        BinaryHeap();
        virtual ~BinaryHeap();

        int sizeVar = 0;
        int* heap = NULL;

        void readFromFile(string filename);
        void showBinaryHeap();
        void addItem(int item);
        int deleteVertex();
        void normalize(int index);
        void initializate(int sizeValue, int elem);
        bool findElement(int i,int item);
        void deleteHeap();

    protected:

    private:
        void addToNormalizedPosition(int element);
};

#endif // BINARYHEAP_H
