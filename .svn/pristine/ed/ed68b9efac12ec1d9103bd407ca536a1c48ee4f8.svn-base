/*
 * Author: Shao Yingxia
 * Create Date: 2014年07月29日 星期二 22时34分47秒
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>
#include <map>
#include <cmath>

using namespace std;

int main(int argc, char** args){
    char* logFile = args[1];
    char* srgsFile = args[2];
    int k = atoi(args[3]);

    FILE* logfp = fopen(logFile, "r");
    FILE* srgsfp = fopen(srgsFile, "r");

    if(logfp == NULL || srgsfp == NULL){
        printf("Fail to open file: %s\n%s\n",logFile, srgsFile);
        return 0;
    }

    char line[1024];
    char graph[1024];
    int cnt = 0;
    vector<int> vid;
    map<int, double> simPair;
    int qv, qnum = 0;
    bool isAns = false;
    double totScore = 0.0;
    
    FILE* outFp;
    sprintf(graph, "%s.ndgc", srgsFile);
    outFp = fopen(graph, "w");

    while(fscanf(srgsfp, "%d:", &qv) != EOF){
        //printf("Query Vid=%d k=%d\n", qv, k);
        vid.clear();
        qnum ++;
        int tmp, tmpk;
        for(int i = 0; i < 50; i++){ //here 50 means the output results is the top-50
            fscanf(srgsfp, "%d", &tmp);
            if(i < k) //only for mod14 and www
                vid.push_back(tmp);
        }
        //extract the perfect ranks
        simPair.clear();
        double nf = 0.0;
        cnt = 0;
        isAns = 0;
        //printf("norm: ");
        while(fgets(line, 1024, logfp) != NULL){
        //    printf("%s cnt=%d isAns=%d qv=%d k=%d\n", line, cnt,isAns, qv, k);
            if(line[0] == 'Q'){
                sscanf(line, "Query(%d,%d)", &tmp, &tmpk);
                if(tmp == qv)
                    isAns = true;
            }
            else if(isAns == true){
                int id; double val;
                sscanf(line, "%d %lf", &id, &val);
                simPair[id] = val;
                cnt++;
                if( cnt <= k){
                    //printf("%.5lf ", val);
                    nf += (pow(2, val) - 1)/ (log(cnt+1)/log(2));
                }
                if(cnt == 100)
                    break;
            }
        }
        //printf("\n");

        //compute ndcg
        double score = 0.0;
        //printf("ndcg: ");
        for(int i = 0; i < k; ++i){
            double v = 0.0;
            int curV = vid[i]; //vid[50-i-1]; //only for mod14 and www
            if(simPair.find(curV) != simPair.end()){
                v = simPair[curV];
            }
            //printf("%.5lf ", v);
            score += (pow(2, v) - 1)/ (log(i+2)/log(2));

        }
        //printf("\n");
        //printf("nf=%.5lf, score=%.5lf\n", nf, score);
        double s= score/nf;
        totScore += s;
        fprintf(outFp, "%d %.5lf\n", qv, s);
    }
    printf("%.5lf\n", totScore / qnum);
    fclose(logfp);
    fclose(srgsfp);
    fclose(outFp);
    return 0;
}

