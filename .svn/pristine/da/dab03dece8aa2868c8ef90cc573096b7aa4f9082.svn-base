/*
 * Author: Shao Yingxia
 * Create Date: 2014年07月29日 星期二 22时34分47秒
 */

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <string>

using namespace std;

int main(int argc, char** args){
    char* filepath = args[1];
    double th = atof(args[2]);

    FILE* fp = fopen(filepath, "r");

    if(fp == NULL){
        printf("Fail to open file: %s\n", filepath);
        return 0;
    }

    char line[1024];
    char graph[1024];
    int cnt = 0;
    vector<int> vid;
    vector<double> sim;
    int qv, k;
    bool isAns = false;

    while(fgets(line, 1024, fp) != NULL){
        //printf("%s cnt=%d isAns=%d qv=%d k=%d\n", line, cnt,isAns, qv, k);
        if(line[0] == 'Q'){
            sscanf(line, "Query(%d,%d)", &qv, &k);
            cnt = 0;
            vid.clear();
            sim.clear();
            isAns = true;
        }
        else if(isAns == true){
             int id; double val;
             sscanf(line, "%d %lf", &id, &val);
             vid.push_back(id);
             sim.push_back(val);
             cnt++;
            
             if(cnt == k){
                isAns = false;
                int vcnt = 0;
                for(int i = 0; i < cnt; ++i){
                    if(sim[i] > th) vcnt++;
                    else break;
                }
                if(vcnt > 0){
                    printf("%s %d %d", graph, qv, vcnt);
                    for(int j = 0; j < vcnt; ++j){
                        printf(" %d", vid[j]);
                    }
                    printf("\n");
                }
                cnt = 0;
             }
        }
        else if(strstr(line, "InputGraph") != NULL){
            strcpy(graph, line);
            graph[strlen(graph) - 1] = '\0';
        }
    }

    return 0;
}

