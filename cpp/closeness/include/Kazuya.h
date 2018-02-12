#ifndef __KAZUYA_H__
#define __KAZUYA_H__

#include "config.h"
#include "adaptiveheap.h"
#include "valuepair.hpp" 
#include "ccinterface.h"
#include <cmath>

class Kazuya: public CCInterface{
	public:
		Kazuya(int* graph_src, int* graph_dst, int mvid):graphSrc(graph_src), graphDst(graph_dst), maxVertexId(mvid) { }

		~Kazuya() {}

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
