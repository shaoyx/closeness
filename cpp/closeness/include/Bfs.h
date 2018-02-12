#ifndef __BFS_H__
#define __BFS_H__

#include "config.h"
#include "adaptiveheap.h"
#include "valuepair.hpp" 
#include "ccinterface.h"
#include <cmath>

class Bfs: public CCInterface{
	public:
		Bfs(int* graph_src, int* graph_dst, int mvid):graphSrc(graph_src), graphDst(graph_dst), maxVertexId(mvid) { }

		~Bfs() {}

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
