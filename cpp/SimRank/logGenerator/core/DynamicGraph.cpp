/*
 * Generation of Dynamic Graph
 */

#include "google/sparse_hash_map"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;

using std::tr1::hash;

struct eqint {
    bool operator() (const int a, const int b) const {
        return a == b;
    }
};


// variables
int edgeNum = -1;
int verticesNum = -1;
int removeEdge = -1;
int MAX_VERTEX_NUM;
char graph[125];
google::sparse_hash_map<int, int, hash<int>, eqint> vertices;
int* edgeSrc;
int* edgeDst;
int* rvertices;


// function
void dynamicGeneration();
void usage();
bool proArgs(int argc, char** args);
void readInitGraph();
int randInt(int start, int end);
int cmp(const void *a, const void *b);
void showCSR(int* gp, int* gi);
void showCSR(int* gp, int* gi, int k);
void indexLogGeneration(int* removeSrc, int* removeDst, int removeCnt, int deleteEdges);

int main (int argc, char** argv) {
	srand((unsigned int) time(0));
	if (!proArgs(argc, argv)) 
		return -1;
	readInitGraph();
	dynamicGeneration();
}

int cmp(const void * a, const void * b){
      return ( *(int*)a - *(int*)b );
}

void readInitGraph() {
    char graphPath[125];
	char processGraphPath[125];
    //char logPath[125];

	//sprintf(graphPath, "dataset/%s/%s.data", graph, graph);
	sprintf(processGraphPath, "dataset/%s/%s.data.fmt", graph, graph);
	//sprintf(logPath, "dataset/%s/removeEdges", graph);
    FILE* fp = fopen(processGraphPath, "rb");

	if (fp == NULL) {
		printf("Can not open file %s.\n", graphPath);
		return;
	}
    	fread(&MAX_VERTEX_NUM, sizeof(int), 1, fp);
    	fread(&edgeNum, sizeof(int), 1, fp);
    	edgeSrc = new int[MAX_VERTEX_NUM + 1];
    	edgeDst = new int[edgeNum];
    	rvertices = new int[MAX_VERTEX_NUM];
    	fread(edgeSrc, sizeof(int), MAX_VERTEX_NUM + 1, fp);
    	fread(edgeDst, sizeof(int), edgeNum, fp);
    	fread(rvertices, sizeof(int), MAX_VERTEX_NUM, fp);

/*    int* cnt = new int[verticesNum + 1];
	edgeSrc = new int[edgeNum];
    edgeDst = new int[edgeNum];
    rvertices = new int[verticesNum];

    vertices.resize(verticesNum);
    memset(cnt, 0, sizeof(int)*(verticesNum + 1));

    int id = 0;
    int ecnt = 0, lcnt = 0;
    int a, b;

	printf("processing each edge.\n");
    while(fscanf(fp, "%d %d", &a, &b) != EOF) {
		// printf("%d %d\n", a, b);
        // relabel each vertex 
        if (vertices.find(a) == vertices.end()) {
            rvertices[id] = a;
            vertices[a] = id ++;
        }
        if (vertices.find(b) == vertices.end()) {
            rvertices[id] = b;
            vertices[b] = id ++;
        }
        
        // self loop found
        if (a == b) lcnt ++;

        edgeSrc[ecnt] = vertices[b];
        edgeDst[ecnt ++] = vertices[a];
        cnt[vertices[b]] ++;
    }

    edgeNum = ecnt;
    MAX_VERTEX_NUM = id;
    fclose(fp);

    // compress step
    int* columnIndex = new int[edgeNum];
    for (int i = 1; i <= MAX_VERTEX_NUM; i ++)
        cnt[i] += cnt[i-1];
    for (int i = 0; i < edgeNum; i ++) {
        -- cnt[edgeSrc[i]];
        columnIndex[ cnt[edgeSrc[i]] ] = edgeDst[i];
    }

    delete [] edgeSrc;
    delete [] edgeDst;
    edgeSrc = cnt;
    edgeDst = columnIndex;

    // delete duplicated edge 
    int np = -1;
    for (int i = 0; i < MAX_VERTEX_NUM; i ++) {
        int s = edgeSrc[i];
        int e = edgeSrc[i+1];
        edgeSrc[i] = np + 1;
        if (s == e) continue;
        qsort(edgeDst+s, e-s, sizeof(int), cmp);
        ++ np;
        edgeDst[np] = edgeDst[s];
        for (int j = s+1; j < e; j ++) {
            if (edgeDst[j] != edgeDst[np]){
                ++ np;
                edgeDst[np] = edgeDst[j];
            }
        }
    }
*/	

    printf("mid = %d en = %d\n", MAX_VERTEX_NUM, edgeNum);
	//showCSR(edgeSrc, edgeDst);
}

int randInt(int start, int end) {
	int length = end - start + 1;
	return rand() % length + start;
}

void dynamicGeneration() {
	int *gp = edgeSrc;
	int *gi = edgeDst;
	
	char processGraphPath[125];
	char removeEdgePath[125];

	sprintf(processGraphPath, "dataset/%s/%s.data.min", graph, graph);
	sprintf(removeEdgePath, "dataset/%s/%s.removeEdge", graph, graph);

	int p, j;

	int k = edgeNum / removeEdge;
	printf("edgeNum %d removeEdge %d.\n", edgeNum, removeEdge);
	printf("k %d.\n", k);
	int end = k * removeEdge;
    srand(time(NULL));
	int rand = randInt(0, k-1);
	printf("rand %d.\n", rand);

	int cnt = 0;
	int removeCnt = 0;

	int *removeSrc = new int[removeEdge];
	int *removeDst = new int[removeEdge];

	// remove edges where 20% edges are remained
    int deleteEdges = (int)(.8*removeEdge);

	for (j = 0; j < MAX_VERTEX_NUM; j ++) {
		
		for (p = gp[j]; p < gp[j+1]; p++) {
			// if condition satisfies
			if (cnt % k == rand && cnt < end) {
				removeSrc[removeCnt] = j;
				removeDst[removeCnt ++] = gi[p];
                if(removeCnt < deleteEdges){ //only delete 80% edges from the original graph
				    gi[p] = -1;
                }
			}
			cnt ++;		
		}
	}


	printf("Remove %d edge.\n", removeCnt);

	//showCSR(gp, gi, k);

	//edgeNum -= removeCnt;

	// save graph in binary format
	printf("saving the modified graph in binary format.\n");
	FILE* fp = fopen(processGraphPath, "wb");
	fwrite(&MAX_VERTEX_NUM, sizeof(int), 1, fp);
	fwrite(&edgeNum, sizeof(int), 1, fp);
	fwrite(edgeSrc, sizeof(int), MAX_VERTEX_NUM + 1, fp);
	fwrite(edgeDst, sizeof(int), edgeNum, fp);
	fwrite(rvertices, sizeof(int), MAX_VERTEX_NUM, fp);
	fclose(fp);
    
    indexLogGeneration(removeSrc, removeDst, removeCnt, deleteEdges);

//	printf("saving remove edges in binary format.\n");
//	fp = fopen(removeEdgePath, "wb");
//	fwrite(&removeCnt, sizeof(int), 1, fp);
//	fwrite(removeSrc, sizeof(int), removeCnt, fp);
//	fwrite(removeDst, sizeof(int), removeCnt, fp);
//	fclose(fp);
}

class Triple{
    public:
    int a, b,c;
    Triple(int a_, int b_, int c_):a(a_), b(b_), c(c_){}
};

void indexLogGeneration(int* removeSrc, int* removeDst, int removeCnt, int deleteEdges){
    //generate modified log
    vector<Triple > logs;
    for(int i = 0; i < removeCnt; ++i){
        if(i < deleteEdges ){ 
            //add logs;
            logs.push_back(Triple(1, removeSrc[i], removeDst[i]));
        }
        else{
            //delete logs
            logs.push_back(Triple(-1, removeSrc[i], removeDst[i]));
        }
    }
    random_shuffle ( logs.begin(), logs.end() );
    for(int s = 0;  s < 100; s++){
	    char logPath[125];

	    sprintf(logPath, "dataset/%s/index/onewaygraph/log.%d", graph, s);
        FILE* fp = fopen(logPath, "w");
        //s samples
        for(int i = 0; i < removeCnt; ++i){
            //update graph
            int op = logs[i].a;
            int src = logs[i].b;
            int dst = logs[i].c;
            if(op == 1){
                for(int p = edgeSrc[src]; p < edgeSrc[src+1]; ++p){
                    if(edgeDst[p] == -1){
                        edgeDst[p] = dst;
             //           printf("ok insert\n");
                        break;
                    }
                }
            }
            else{
                for(int p = edgeSrc[src]; p < edgeSrc[src+1]; ++p){
                    if(edgeDst[p] == dst){
                        edgeDst[p] = -1;
            //            printf("ok delete\n");
                        break;
                    }
                }
            }
            //generate log
            vector<int> nbr;
            for(int p = edgeSrc[src]; p < edgeSrc[src+1]; ++p){
                if(edgeDst[p] != -1){
                    nbr.push_back(edgeDst[p]);
                }
            }
            int rv = -1;
            if(nbr.size() > 0){
                rv = nbr[randInt(0, nbr.size()-1)];
            }
            fprintf(fp,"%d %d %d %d\n", op, src, dst, rv);
        }
        fclose(fp);
        
        for(int i = 0; i < removeCnt; ++i){
            //update graph
            int op = logs[i].a;
            int src = logs[i].b;
            int dst = logs[i].c;
            if(op == -1){
                for(int p = edgeSrc[src]; p < edgeSrc[src+1]; ++p){
                    if(edgeDst[p] == -1){
                        edgeDst[p] = dst;
                        break;
                    }
                }
            }
            else{
                for(int p = edgeSrc[src]; p < edgeSrc[src+1]; ++p){
                    if(edgeDst[p] == dst){
                        edgeDst[p] = -1;
                        break;
                    }
                }
            }
        }
    }
}

void usage() {
    printf("App Usage:\n");
    printf("\t-g  <string>, the name of input graph.\n");
    printf("\t-en  <int>, the number of edges in the input graph.\n");
    printf("\t-vn  <int>, the number of vertices in the input graph.\n");
    printf("\t-m  <string>, specify the method of computing SimRank.\n");
    printf("\t-bi , only build index.\n");
    printf("\t-ud <int>, specify using disk to store the sampled graph [0=original, 1=disk, 2=compress].\n");
    printf("\t-hi, specify the samples has been built.\n");
    printf("\t-fm  <bool>, specify whether first-meeting guarantee or not. [true]\n");
    printf("\t-iv  <double>, specify the initial values. [1.0]\n");
    printf("\t-sn  <int>, specify number of global sampling. [100]\n");
    printf("\t-sqn  <int>, specify the number of local sampling. [10]\n");
    printf("\t-c  <string>, specify the file of configuration. [config]\n");
}

bool proArgs(int argc, char** args) {
    //1. process the options
    int i;
    for(i = 1; i < argc; i++)
    {
		if ( i+1 > argc)
			break;

        if(strcmp(args[i], "-g") == 0) {
        	++i;
            strcpy(graph, args[i]);
        }
    
        else if(strcmp(args[i], "-en") == 0) {
        	++i;
        	edgeNum = atoi(args[i]);
        }

        else if(strcmp(args[i], "-vn") == 0) {
        	++i;
        	verticesNum = atoi(args[i]);
        }

		else if(strcmp(args[i], "-re") == 0) {
			++i;
			removeEdge = atoi(args[i]);
		}

        else {
			printf("invalid parameter: %s\n", args[i]);
            usage();
            return false;
        }
    }

    if(strlen(graph) == 0  || edgeNum == -1 || verticesNum == -1) {
    	printf("No Input Graph or Testing Method! or edgeNum and vertciesNum are not specified.\n");
        usage();
        return false;
    }
	return true;
}

void showCSR(int* gp, int* gi) {
	int j;
	for (j = 0; j <= MAX_VERTEX_NUM; j++) 
		printf("%d ", gp[j]);
	printf("\n");

	int nz = gp[MAX_VERTEX_NUM];
	for (j = 0; j < nz; j ++)
		printf("%d ", gi[j]);
	printf("\n");
}

void showCSR(int* gp, int* gi, int k) {
	int j;
	for (j = 0; j <= MAX_VERTEX_NUM; j++) 
		printf("%d ", gp[j]);
	printf("\n");

	int nz = gp[MAX_VERTEX_NUM];
	for (j = 0; j < nz; j ++) {
		printf("%d ", gi[j]);
		if (j % k == 0 && j != 0)
			printf("\n");
	}
	printf("\n");
}
