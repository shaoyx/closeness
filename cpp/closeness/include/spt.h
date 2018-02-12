#ifndef __SPT_H__
#define __SPT_H__

#include "config.h"
#include "adaptiveheap.h"
#include "valuepair.hpp" 
#include "ccinterface.h"

class SPT : public CCInterface{
	public:
		SPT(int* graph_src, int* graph_dst, int mvid):graphSrc(graph_src), graphDst(graph_dst), maxVertexId(mvid) { }

		~SPT() {}

		virtual void run(int k);
        virtual vector<ValuePair> getResults();

	private:
        void select_k_vertices(int k, int* seed);
		int bfs(int s, int* vis, int ts, int* dist, int* visSeq, int& ccSize);
		void computeSpBound(int sv, int spSum, int* dist, int* visSeq, int ccSize, double* cc_bound);
		void updateSpBound(int sv, int spSum, int* dist, int* visSeq, int ccSize, double* cc_bound, AdaptiveHeap* heap);
        void printArray(int* array, int size);
        void printArray(double* array, int size);
        
	private:
		int* graphSrc;
		int* graphDst;
		int maxVertexId;
        vector<ValuePair> results;
};

#endif
