#ifndef __ADAPTIVEHEAP_H__
#define __ADAPTIVEHEAP_H__

#include "config.h"

struct element {
    int id;
    double weight;
};

/**
* It implementes a max-heap class which automatically updates the
* heap structure when the internal elements are updated.
**/
class AdaptiveHeap {
	public:
		AdaptiveHeap():heapSize(0), id2pos(NULL), heapArray(NULL) {}
		~AdaptiveHeap() {
            if(id2pos != NULL){
                delete id2pos;
            }
            if(heapArray != NULL){
                delete heapArray;
            }
        }
		void buildHeap(double* weight, int asize, int vsize);
		bool isEmpty();
		element pop();
		void update(int id, double weight);
        void print();
		
	private:
		void heapifyUp(int root);
        void heapifyDown(int root);
		void build();
		void swap(int first, int second);

	private:
		int heapSize;
		int* id2pos;
		element* heapArray;
};

#endif
