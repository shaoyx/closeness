#include "adaptiveheap.h"

void AdaptiveHeap::buildHeap(double* weight, int asize, int vsize){
	heapSize = asize;
	heapArray = new element[heapSize + 1];
	id2pos = new int[vsize + 1];
	for(int i = 1; i <= heapSize; i++){
		heapArray[i].id = i-1;
        heapArray[i].weight = (weight[i-1] < 0 ? 1e10 : weight[i-1]);
		id2pos[heapArray[i].id] = i;
	}
	build(); //build the heap with maintaining id2pos map.
}

bool AdaptiveHeap::isEmpty(){
	return heapSize == 0;
}

element AdaptiveHeap::pop(){
	int resPos = heapSize;
	swap(1, heapSize);
	heapSize--;
	heapifyDown(1);
	return heapArray[resPos];
}

void AdaptiveHeap::update(int id, double weight){
    /* only work for max-heap */
    if(id2pos[id] <= heapSize && heapArray[id2pos[id]].weight > weight){
	   heapArray[id2pos[id]].weight = weight;
	   heapifyDown(id2pos[id]);
    }
}

/******* private methods *******/
void AdaptiveHeap::heapifyUp(int root){
    /* max-heap */
	int child = root;
	int parent = root / 2;
	if(parent == 0 || heapArray[parent].weight >= heapArray[child].weight) 
		return;
	swap(parent, child);
	heapifyUp(parent);
}

void AdaptiveHeap::heapifyDown(int root){
	int max = root;
	int left  = root * 2;
	int right = root * 2 + 1;
	if(left <= heapSize && heapArray[max].weight < heapArray[left].weight)
		max = left;
	if(right <= heapSize && heapArray[max].weight < heapArray[right].weight)
		max = right;
	if(max != root){
		swap(max, root);
		heapifyDown(max);
	}
}
		
void AdaptiveHeap::build(){
	int pos = heapSize / 2;
	while(pos > 0){
		heapifyDown(pos);
		pos--;
	}
}

void AdaptiveHeap::swap(int first, int second){
	double tmpweight = heapArray[first].weight;
	int tmpid = heapArray[first].id;
	heapArray[first].weight = heapArray[second].weight;
	heapArray[first].id = heapArray[second].id;
	id2pos[heapArray[second].id] = first; //record the id2pos here.
	
	heapArray[second].weight = tmpweight;
	heapArray[second].id = tmpid;
	id2pos[tmpid] = second; // record the id2pos here.
}

void AdaptiveHeap::print(){
    printf("HeapSize=%d.\n", heapSize);
    for(int i = 1; i <= heapSize; i++){
        printf("(%dth: %d %.5lf)\n", i, heapArray[i].id, heapArray[i].weight);
    }
    printf("\n");
}
