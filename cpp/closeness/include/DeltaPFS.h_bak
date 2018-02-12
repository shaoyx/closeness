#ifndef __DPFS_H__
#define __DPFS_H__

#include "config.h"
#include "adaptiveheap.h"
#include "valuepair.hpp" 
#include "ccinterface.h"

class DeltaPFS : public CCInterface{
	public:
		DeltaPFS(int* graph_src, int* graph_dst, int mvid):graphSrc(graph_src), graphDst(graph_dst), maxVertexId(mvid) { }

		~DeltaPFS() {}

		virtual void run(int k);
        virtual vector<ValuePair> getResults();

	private:
        
	private:
		int* graphSrc;
		int* graphDst;
		int maxVertexId;
        vector<ValuePair> results;
};

#endif
