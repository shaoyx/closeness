/*
 * dynamicowgmanager.h
 *
 *  Created on: 2014-8-25
 *      Author: simon0227
 */

#ifndef _DYNAMICOWGMANAGER_H_
#define _DYNAMICOWGMANAGER_H_

#include "config.h"
#include "onewaygraph.h"
#include "gsinterface.h"

class DynamicOwgManager: public GSInterface{
public:
	DynamicOwgManager(int sn, int mvid, char* prefix) :
		sampleGraphNum(sn), maxVertexId(mvid) {
		owg = new OneWayGraph*[sn];
        for(int i = 0; i < sn; i++)
		    owg[i] = new OneWayGraph(prefix);
	}

	~DynamicOwgManager(){
        for(int i = 0; i < sampleGraphNum; i++)
		   delete owg[i];
		delete [] owg;
	}

	void initialize();

	/**
	 * This insertEdge is called during the building.
	 */
	void insertEdge(int sid, int src, int dst);

	void save(int sid);

	/* process sampleId one-way graph for computing SimRank scores. */
	void computeSimrank(int sid, vector<SimRankValue>& sim, map<int, vector<int>* >& timestamp, int maxSteps, double df, int qv);

	/* update API */
    void loadIndex(int sid);
    void mergeLog(int sid);
	void update(int sid, int src, int dst);

private:
	int sampleGraphNum;
	int maxVertexId;
	//int sampleId;
	OneWayGraph** owg;
};


#endif /* _DYNAMICOWGMANAGER_H_ */
