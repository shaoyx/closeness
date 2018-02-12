/*
 * logmanager.h
 *
 *  Created on: 2014-8-16
 *      Author: ggbond
 */

#ifndef LOGMANAGER_H_
#define LOGMANAGER_H_

#include "config.h"

class LogManager{
public:
	static int MAX_LOG_RECORD_NUM;
	static int NODE_DELETED_FLAG;
	static int NODE_ADDED_FLAG;
    static int NULL_NODE;

	LogManager():logStore(NULL), logDst(NULL), curLogNum(0), maxVertexId(-1), freeTail(0),
			startIdx(NULL), pointList(NULL), neighborList(NULL), hasNewLog(false){
	}

	~LogManager(){
		delete [] logStore;
//		delete [] logDst; //TODO: have bug for deleting the following two lines.
//		delete [] startIdx;
		delete [] neighborList;
		delete [] pointList;
	}

	void initialize(int mvid){
		curLogNum = 0;
		maxVertexId = mvid;
		freeTail = 0;
		hasNewLog = false;
		logStore = new int[MAX_LOG_RECORD_NUM];
		logDst = new int[maxVertexId];
		memset(logDst, -1, sizeof(int)*maxVertexId );
		startIdx = new int[maxVertexId];
		neighborList = new int[MAX_LOG_RECORD_NUM];
		pointList = new int[MAX_LOG_RECORD_NUM];
		memset(startIdx, -1, sizeof(int)*maxVertexId );
	}

	void insertLog(int src, int dst);
	bool isChanged(int vid, int dst); /* node vid changes its dst vertex. */
	bool isDeletedNode(int vid);
	bool isNewNode(int vid);
	void getNewNeighbors(int vid, vector<int>& nb);
    void getNewNeighbors(int vid, int* nb, int& offset);
	bool hasUpdated() { return hasNewLog; }
	
    bool isEmpty() { return curLogNum == 0; }
    void clear(){
        curLogNum = 0;
        freeTail = 0;
    }

	void save(char* filepath);
	void read(char* filepath);

	void print();
    void print(int vid);

private:
	int* logStore; /* store the log */
	int* logDst; /* a array-based map */
	int curLogNum;

	int maxVertexId;
	int freeTail;
	int* startIdx;
	int* pointList;
	int* neighborList;

	bool hasNewLog;
};


#endif /* LOGMANAGER_H_ */
