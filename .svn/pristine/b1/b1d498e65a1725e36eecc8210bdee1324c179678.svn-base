#ifndef __Sketch_H__
#define __Sketch_H__

#include "config.h"
#include "adaptiveheap.h"
#include "valuepair.hpp" 
#include "ccinterface.h"

class Sketch : public CCInterface{
	public:
		Sketch(int* graph_src, int* graph_dst, int mvid):graphSrc(graph_src), graphDst(graph_dst), maxVertexId(mvid) { }

		~Sketch() {}

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
