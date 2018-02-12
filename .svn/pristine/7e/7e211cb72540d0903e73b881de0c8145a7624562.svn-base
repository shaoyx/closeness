/*
 * Compute SimRank based on the Random Surfer Model.
 * This version both support the first-meeting points and non-first-meeting points,
 * with different initialized values.
 */
#include "config.h"
#include "srgs.h"
#include "google/dense_hash_map"
#include "google/sparse_hash_map"

using google::dense_hash_map;
using std::tr1::hash;

/* method flags. */
char SRGSM[10] = "srgs";

struct eqint {
    bool operator()(const int a, const int b) const {
        return a == b;
    }
};

int MAX_VERTEX_NUM; // vertex id is labeled [0, MAX_VERTEX_NUM)
//map<int, vector<int> > in_graph;
int* graph_src;
int* graph_dst;
int* orig_graph_src;
int* orig_graph_dst;
int verticesNum = -1, edgeNum = -1;
//google::dense_hash_map<int, int, hash<int>, eqint> vertices;
google::sparse_hash_map<int, int, hash<int>, eqint> vertices;
int* rvertices;
bool hasIndex = false;
bool buildIndex = false;
bool queryInFile = false;

/* configuration */
char inpath[125] = "";
char config_file[125] = "config";
char method[56] = "";
bool isFm = true;
bool needOrig = false;
int usDisk = 0;
double initValue = 1.0;

/* reading from config file */
int numIter = 11; // default
double decayFactor = 0.8; // default
int sampleNum = 100;
int sampleQueryNum = 10;

/** function **/
bool pro_args(int argc, char** args);
void read_config();
void read_inputgraph();
void do_computation(int vid, int k, SimRankMethod* srm);

/*
 * Reverse the graph with relabeling it.
 */
int main(int argc, char** argv){

	if( !pro_args(argc, argv) )
		return 0;

	read_config();

    usDisk=3; //use the one-way graph.

    printf("InputGraph=%s;\nnum_iter=%d;\ndecay_factor=%.4lf\nsampleNum=%d;\nsampleQueryNum=%d;\nis_fm=%s;\nuDisk=%d;\nhasIndex=%d;\nbuildIndex=%d;\nneedOrig=%d;\n\n",
    		inpath, numIter, decayFactor, sampleNum, sampleQueryNum, (isFm == 0 ? "true" : "false"), usDisk, hasIndex, buildIndex, needOrig);

    Time timer;
    timer.start();
	read_inputgraph();
	timer.stop();
	printf("Time cost for reading graph: %.5lf\n", timer.getElapsedTime());

    SimRankMethod* srm = NULL;

	char sgpath[125];
    sprintf(sgpath, "dataset/%s/index", inpath);
	srm = new SRGS(numIter, sampleNum, decayFactor, sampleQueryNum,
			graph_src, graph_dst, MAX_VERTEX_NUM, usDisk, sgpath, hasIndex);

	if(buildIndex){
		timer.reset();
		timer.start();
		srm->initialize();
        timer.stop();
		printf("Method=%s, graph = %s, udsik=%d: Time cost for build index: %.5lf\n", method, inpath, usDisk, timer.getElapsedTime());
		return 0;
	}

    /*insert log*/
    printf("loading index ... ...\n");
    char logPath[126];

    srm->loadIndex();
    printf("beginning update index ... ...\n");
    timer.reset();
    timer.start();
    Time tt;
    double upgraph=0;
//    sampleNum = 1;
    for(int sid = 0; sid < sampleNum; sid++){
        sprintf(logPath, "dataset/%s/index/onewaygraph/log.%d", inpath, sid);
        FILE* fp = fopen(logPath, "r");
//        Time timer;
//        timer.start();
        /*update the sid index*/
        int op, s, e, ne;
        int cnt = 0;
        while(fscanf(fp, "%d %d %d %d", &op, &s, &e, &ne) != EOF){
//            printf("Log(%d, %d %d %d)\n", op,s,e,ne);
            tt.reset(); 
            tt.start(); 
            cnt++;
            if(op == 1){
                for(int p = graph_src[s]; p < graph_src[s+1]; ++p){
                    if(graph_dst[p] == -1){
                        graph_dst[p] = e;
                        break;
                    }
                }
            }
            else{
                for(int p = graph_src[s]; p < graph_src[s+1]; ++p){
                    if(graph_dst[p] == e){
                        graph_dst[p] = -1;
                        break;
                    }
                }
            }
            tt.stop();
            upgraph += tt.getElapsedTime();

            srm->update(sid, s, ne);
        }
 //       timer.stop();
        fclose(fp);
    }
    timer.stop();
    double totTime = timer.getElapsedTime();
	printf("Update Cost: %.5lf upGraph: %.5lf accy UpdateCost=%.5lf\n", totTime, upgraph, totTime - upgraph);
    printf("beginning merge log ... ...\n");
    timer.reset();
    timer.start();

    srm->mergeLog();
    timer.stop();
    totTime = timer.getElapsedTime();
	printf("Merge Cost: %.5lf\n", totTime);
    
    return 0;

    printf("beginning query ... ...\n");
 
    /*begin query*/
	int qv;
	int k;

//	printf("Input query(qv, k):\n");
    timer.reset();
    timer.start();

    int qcnt = 0;
    if(queryInFile){
        char querypath[125];
        sprintf(querypath, "dataset/%s/%s.out", inpath,inpath);
        FILE* qfp = fopen(querypath, "rb");
        k=50;
        printf("querypath=%s\n", querypath);
        if(qfp == NULL){
            printf("failed to open file\n");
        }
    	while(fread(&qv, sizeof(int), 1, qfp) == 1){
            printf("Query(%d,%d,%d, deg=%d):\n", qv, k, vertices[qv], graph_src[vertices[qv]+1]-graph_src[vertices[qv]]);
		    do_computation(vertices[qv], k, srm);
            qcnt++;
        }
        if(qfp != NULL)
            fclose(qfp);
    }
    else{
	    while(scanf("%d %d", &qv, &k) != EOF){
            printf("Query(%d,%d,%d, deg=%d):\n", qv, k, vertices[qv], graph_src[vertices[qv]+1]-graph_src[vertices[qv]]);
		    do_computation(vertices[qv], k, srm);
            qcnt++;
	    }
    }
	timer.stop();
	printf("Time cost for executing %d queires: %.5lf\n", qcnt, timer.getElapsedTime());

    delete srm;

    return 0;
}

void do_computation(int qv, int k, SimRankMethod* srm){
    srm->run(qv, k);
  	fprintf(stderr, "%d:", rvertices[qv]);
      for(int i = 0; i < k; ++i){
          int vid = srm->getRes(i).getVid();
          double val = srm->getRes(i).getValue()/sampleNum/sampleQueryNum; 
          if(vid != -1){
      		fprintf(stderr, " %d", rvertices[vid]);
              printf("%d %.5lf\n", rvertices[vid], val);
          }
          else{
      		fprintf(stderr, " -1");
              printf("-1 -1\n");
              //printf("No qualified vertex for %dth pos\n", i);
          }
      }
  	fprintf(stderr, "\n");
      if(usDisk == 1 || usDisk == 2 || usDisk == 3)
          hasIndex = true;
}

int cmp(const void * a, const void * b){
      return ( *(int*)a - *(int*)b );
}

/* read the graph with reversing and relabeling the graph.*/
void read_inputgraph(){
	char originalGraphPath[125];
	char processedGraphPath[125];
	char orig_processedGraphPath[125];

	sprintf(originalGraphPath, "dataset/%s/%s.data",inpath, inpath);
	sprintf(processedGraphPath, "dataset/%s/%s.data.min",inpath, inpath);
	sprintf(orig_processedGraphPath, "dataset/%s/%s.data.fmt.orig",inpath, inpath);

    FILE* fp = fopen(processedGraphPath, "rb");
    if( fp != NULL ){
    	printf("reading from processed graph path: %s\n", processedGraphPath);

    	/* read from the binary file */
    	fread(&MAX_VERTEX_NUM, sizeof(int), 1, fp);
    	fread(&edgeNum, sizeof(int), 1, fp);
    	graph_src = new int[MAX_VERTEX_NUM + 1];
    	graph_dst = new int[edgeNum];
    	rvertices = new int[MAX_VERTEX_NUM];
    	fread(graph_src, sizeof(int), MAX_VERTEX_NUM + 1, fp);
    	fread(graph_dst, sizeof(int), edgeNum, fp);
    	fread(rvertices, sizeof(int), MAX_VERTEX_NUM, fp);
    	vertices.resize(MAX_VERTEX_NUM);
    	for(int i = 0; i < MAX_VERTEX_NUM; ++i){
    		vertices[rvertices[i]] = i;
    	}
    	fclose(fp);
    	if(needOrig == true){
    		fp = fopen(orig_processedGraphPath, "rb");
    		if(fp == NULL){
				 /*construct the original version */
				orig_graph_src = new int [edgeNum];
				orig_graph_dst = new int [edgeNum];
				int* tcnt = new int[MAX_VERTEX_NUM + 1];
				memset(tcnt, 0, sizeof(int)*(MAX_VERTEX_NUM + 1));
				int et = 0;
				for(int i = 0; i < MAX_VERTEX_NUM; ++i){
					int s = graph_src[i];
					int e = graph_src[i+1];
					if(e == s){
						continue;
					}
					for(int j = s; j < e; ++j){
						tcnt[graph_dst[j]]++;
						orig_graph_src[et] = graph_dst[j];
						orig_graph_dst[et] = i;
						et++;
					}
				}
				int* otmp = new int[edgeNum];
				for(int i = 1; i <= MAX_VERTEX_NUM; ++i) tcnt[i] += tcnt[i-1];
				for(int i = 0; i < edgeNum; ++i){
					--tcnt[orig_graph_src[i]];
					otmp[tcnt[orig_graph_src[i]]] = orig_graph_dst[i];
				}
				delete [] orig_graph_src;
				delete [] orig_graph_dst;
				orig_graph_src = tcnt;
				orig_graph_dst = otmp;
				
                fp = fopen(orig_processedGraphPath, "wb");
		    	fwrite(&MAX_VERTEX_NUM, sizeof(int), 1, fp);
		    	fwrite(&edgeNum, sizeof(int), 1, fp);
		    	fwrite(orig_graph_src, sizeof(int), MAX_VERTEX_NUM + 1, fp);
		    	fwrite(orig_graph_dst, sizeof(int), edgeNum, fp);
		    	fclose(fp);
    		}
    		else{
    			int a,b;
    			fread(&a, sizeof(int), 1, fp);
				fread(&b, sizeof(int), 1, fp);
				orig_graph_src = new int[MAX_VERTEX_NUM + 1];
				orig_graph_dst = new int[edgeNum];
				fread(orig_graph_src, sizeof(int), MAX_VERTEX_NUM + 1, fp);
				fread(orig_graph_dst, sizeof(int), edgeNum, fp);
				fclose(fp);
    		}
    	}
        printf("[input]EdgeNum=%d, inVertexNum=%d.\n", edgeNum, MAX_VERTEX_NUM);

    }else{
    	printf("reading from original graph path: %s\n", originalGraphPath);
    	fp = fopen(originalGraphPath, "r");
    	int a,b;
    	MAX_VERTEX_NUM = -1;
    	int id = 0;
    	int ecnt = 0, lcnt = 0;
    	printf("require the EdgeList format.\n");

    	int* cnt = new int[verticesNum + 1];
    	graph_src = new int[edgeNum];
    	graph_dst = new int[edgeNum];
    	rvertices = new int[verticesNum];

     //vertices.set_empty_key(-1);
    	vertices.resize(verticesNum);

    	memset(cnt, 0, sizeof(int)*(verticesNum + 1));
     
    	printf("before reading in graph: meminfo ");
    	//print_mem_info();

    	while(fscanf(fp, "%d %d",&a, &b) != EOF){
    		if(vertices.find(a) == vertices.end()) {rvertices[id] = a; vertices[a] = id++;} // relabel
    		if(vertices.find(b) == vertices.end()) {rvertices[id] = b; vertices[b] = id++;}
            if(a == b) { lcnt++; } //continue;} //delete loop
    		graph_src[ecnt] = vertices[b]; //reverse the graph here.
    		graph_dst[ecnt] = vertices[a];
    		cnt[vertices[b]]++;
    		ecnt++;
    	}
        edgeNum = ecnt;
    	MAX_VERTEX_NUM = id;
    	fclose(fp);

    	printf("reading in graph: meminfo ");
    	//print_mem_info();

    	int* tmp = new int[edgeNum];
    	for(int i = 1; i <= MAX_VERTEX_NUM; ++i) cnt[i] += cnt[i-1];
    	for(int i = 0; i < edgeNum; ++i){
    		--cnt[graph_src[i]];
    		tmp[cnt[graph_src[i]]] = graph_dst[i];
    	}
    	delete[] graph_src;
    	delete[] graph_dst;
    	graph_src = cnt;
    	graph_dst = tmp;

        /*delete the duplicated edge here!!!*/
     if(MAX_VERTEX_NUM < 10){
         for(int i = 0; i <= MAX_VERTEX_NUM; ++i){
             printf("(%d, %d) ", i, graph_src[i]);
         }
         printf("\n");
         for(int i = 0; i < edgeNum; ++i){
             printf("%d ", graph_dst[i]);
         }
         printf("\n");
     }
        int np = -1;
        for(int i = 0; i < MAX_VERTEX_NUM; ++i){
            int s = graph_src[i];
            int e = graph_src[i+1];
            graph_src[i] = np + 1;
            if(e == s){
                continue;
            }
            qsort(graph_dst+s, e-s, sizeof(int), cmp);
            ++np;
            graph_dst[np] = graph_dst[s];
            for(int j = s+1; j < e; ++j){
                if(graph_dst[j] != graph_dst[np]){
                    ++np;
                    graph_dst[np] = graph_dst[j];
                }
            }
        }
        printf(" mvid =%d en = %d ien=%d\n", MAX_VERTEX_NUM, np+1, edgeNum);
        graph_src[MAX_VERTEX_NUM] = np + 1;
        edgeNum = np + 1;

        /*construct the original version */
        orig_graph_src = new int [edgeNum];
        orig_graph_dst = new int [edgeNum];
        int* tcnt = new int[MAX_VERTEX_NUM + 1];
    	memset(tcnt, 0, sizeof(int)*(MAX_VERTEX_NUM + 1));
        int et = 0;
        for(int i = 0; i < MAX_VERTEX_NUM; ++i){
            int s = graph_src[i];
            int e = graph_src[i+1];
            if(e == s){
                continue;
            }
            for(int j = s; j < e; ++j){
            	tcnt[graph_dst[j]]++;
            	orig_graph_src[et] = graph_dst[j];
            	orig_graph_dst[et] = i;
            	et++;
		    }
        }
        int* otmp = new int[edgeNum];
		for(int i = 1; i <= MAX_VERTEX_NUM; ++i) tcnt[i] += tcnt[i-1];
		for(int i = 0; i < edgeNum; ++i){
			--tcnt[orig_graph_src[i]];
			otmp[tcnt[orig_graph_src[i]]] = orig_graph_dst[i];
		}
		delete [] orig_graph_src;
		delete [] orig_graph_dst;
		orig_graph_src = tcnt;
		orig_graph_dst = otmp;

    	//save graph in binary format here.
    	printf("saving the graph in binary format.\n");
    	fp = fopen(processedGraphPath, "wb");
    	fwrite(&MAX_VERTEX_NUM, sizeof(int), 1, fp);
    	fwrite(&edgeNum, sizeof(int), 1, fp);
    	fwrite(graph_src, sizeof(int), MAX_VERTEX_NUM + 1, fp);
    	fwrite(graph_dst, sizeof(int), edgeNum, fp);
    	fwrite(rvertices, sizeof(int), MAX_VERTEX_NUM, fp);
    	fclose(fp);
    	fp = fopen(orig_processedGraphPath, "wb");
    	fwrite(&MAX_VERTEX_NUM, sizeof(int), 1, fp);
    	fwrite(&edgeNum, sizeof(int), 1, fp);
    	fwrite(orig_graph_src, sizeof(int), MAX_VERTEX_NUM + 1, fp);
    	fwrite(orig_graph_dst, sizeof(int), edgeNum, fp);
    	fclose(fp);
        printf("[input]EdgeNum=%d, inVertexNum=%d. [online]mvid=%d, edge=%d, loop=%d\n", edgeNum, verticesNum, MAX_VERTEX_NUM, ecnt, lcnt);
    }

     if(MAX_VERTEX_NUM < 10){
         for(int i = 0; i <= MAX_VERTEX_NUM; ++i){
             printf("(%d, %d) ", i, graph_src[i]);
         }
         printf("\n");
         for(int i = 0; i < edgeNum; ++i){
             printf("%d ", graph_dst[i]);
         }
         printf("\n");
         if(needOrig == true){
             printf("orignal graph\n");
            for(int i = 0; i <= MAX_VERTEX_NUM; ++i){
                printf("(%d, %d) ", i, orig_graph_src[i]);
            }
            printf("\n");
            for(int i = 0; i < edgeNum; ++i){
                printf("%d ", orig_graph_dst[i]);
            }
            printf("\n");
         }
     }


}

void help() {
    printf("App Usage:\n");
    printf("\t-g  <string>, the name of input graph.\n");
    printf("\t-en  <int>, the number of edges in the input graph.\n");
    printf("\t-vn  <int>, the number of vertices in the input graph.\n");
    printf("\t-m  <string>, specify the method of computing SimRank.\n");
    printf("\t-bi , only build index.\n");
    printf("\t-qf , indicate query in the file.\n");
    printf("\t-no , need keep the original graph in memory.\n");
    printf("\t-ud <int>, specify using disk to store the sampled graph [0=original, 1=disk, 2=compress].\n");
    printf("\t-hi, specify the samples has been built.\n");
    printf("\t-fm  <bool>, specify whether first-meeting guarantee or not. [true]\n");
    printf("\t-iv  <double>, specify the initial values. [1.0]\n");
    printf("\t-sn  <int>, specify number of global sampling. [100]\n");
    printf("\t-sqn  <int>, specify the number of local sampling. [10]\n");
    printf("\t-c  <string>, specify the file of configuration. [config]\n");
}

bool pro_args(int argc, char** args) {
    //1. process the options
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(args[i], "-g") == 0 && i + 1 < argc) {
        	++i;
            strcpy(inpath, args[i]);
        }
        else if(strcmp(args[i], "-m") == 0 && i + 1 < argc){
        	++i;
        	strcpy(method, args[i]);
        }
        else if(strcmp(args[i], "-fm") == 0 && i + 1 < argc) {
        	++i;
        	isFm = (strcmp(args[i],"true") == 0 ? true : false);
        }
        else if(strcmp(args[i], "-bi") == 0) {
        	buildIndex = true;
        }
        else if(strcmp(args[i], "-no") == 0) {
        	needOrig = true;
        }
        else if(strcmp(args[i], "-ud") == 0 && i + 1 < argc) {
        	++i;
        	usDisk = atoi(args[i]);
        }
        else if(strcmp(args[i], "-qf") == 0) {
        	queryInFile = true;
        }
        else if(strcmp(args[i], "-hi") == 0) {
        	hasIndex = true;
        }
        else if(strcmp(args[i], "-iv") == 0 && i + 1 < argc) {
        	++i;
        	initValue = atof(args[i]);
        }
        else if(strcmp(args[i], "-sn") == 0 && i + 1 < argc) {
        	++i;
        	sampleNum = atoi(args[i]);
        }
        else if(strcmp(args[i], "-en") == 0 && i + 1 < argc) {
        	++i;
        	edgeNum = atoi(args[i]);
        }
        else if(strcmp(args[i], "-vn") == 0 && i + 1 < argc) {
        	++i;
        	verticesNum = atoi(args[i]);
        }
        else if(strcmp(args[i], "-sqn") == 0 && i + 1 < argc) {
        	++i;
        	sampleQueryNum = atoi(args[i]);
        }
        else if(strcmp(args[i], "-c") == 0 && i + 1 < argc){
        	++i;
        	strcpy(config_file, args[i]);
        }
        else {
			printf("invalid parameter: %s\n", args[i]);
            help();
            return false;
        }
    }

    if(strlen(inpath) == 0 || strlen(method) == 0 || edgeNum == -1 || verticesNum == -1) {
    	printf("No Input Graph or Testing Method! or edgeNum and vertciesNum are not specified.\n");
        help();
        return false;
    }
	return true;
}

void read_config(){
	FILE* fp = fopen(config_file, "r");
    char line[1024];
	char key[128];
	char value[128];
	while(fgets(line, 1024, fp) != NULL){
        sscanf(line, "%s %s", key, value);
//        printf("%s\n", key);
		if(strcmp("decay_factor", key) == 0){
			decayFactor = atof(value);
		}
		else if(strcmp("num_iter", key) == 0){
			numIter = atoi(value);
		}
//       printf("%s %lf %d\n", key, decayFactor, numIter);
	}
	fclose(fp);
}
