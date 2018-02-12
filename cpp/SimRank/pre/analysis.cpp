/*
 * Author: Shao Yingxia
 * Create Date: 2014年07月30日 星期三 10时37分55秒
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char** args){
//	printf("Input ccPath, apprPath and topk.\n");
    char* ccPath = args[1];
    char* apprPath = args[2];
    int topk = atoi(args[3]);

    FILE* ccFp = fopen(ccPath, "r");
    FILE* apprFp = fopen(apprPath, "r");

    char graph[1024];
    int vid, num;
    vector<int> cc;
    vector<int> appr;
    FILE* outFp;
    sprintf(graph, "%s.percentage", apprPath);
    outFp = fopen(graph, "w");

    int qnum = 0;
    double average=0;

    while(fscanf(ccFp, "%s", graph) != EOF){
        cc.clear();
        appr.clear();
        fscanf(ccFp, "%d %d", &vid, &num);
        int tmp;
        for(int i = 0; i < num; i++){
            fscanf(ccFp, "%d", &tmp);
            if(i < topk){
                cc.push_back(tmp);
            }
        }
        fscanf(apprFp, "%s", graph);
        for(int i = 0; i < 500; i++){ //here 50 means the output results is the top-50
            fscanf(apprFp, "%d", &tmp);
//            if(50 - i <= topk)
            if(i < topk)
            	appr.push_back(tmp);
        }
        sort(cc.begin(), cc.end());
        sort(appr.begin(), appr.end());
        int i = 0, j = 0;
        int tc = 0;
        while(i < topk){
           while(j < topk && appr[j] < cc[i]) ++j;
           if(appr[j] == cc[i]) tc++;
           i++; 
        }
        qnum++;
        average += tc/(1.0*topk);
        fprintf(outFp, "%d %.5lf\n", vid, tc / (1.0*topk));
    }

    printf("%.5lf\n", average/qnum);
    fclose(ccFp);
    fclose(apprFp);
    fclose(outFp);

    return 0;
}
