/*
 * reverse the original graph with relabeling the graph
 * with successive id.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

map<int, vector<int> > graph;
map<int, int> vertices;

int main(int argc, char** argv){

    char* inpath = argv[1];
    char* outpath = argv[2];

    freopen(inpath, "r", stdin);
    freopen(outpath, "w", stdout);

    graph.clear();
    
    int a,b;
    int id = 0;

    /* relabel the id */
    while(scanf("%d %d",&a,&b) != EOF){
        graph[b].push_back(a);
        if(vertices.find(a) == vertices.end()) {vertices[a] = id++;}
        if(vertices.find(b) == vertices.end()) {vertices[b] = id++;}
    }

    map<int, vector<int> >::iterator iter;

    for(iter = graph.begin(); iter != graph.end(); ++iter){
        vector<int> nb = iter->second;
        printf("%d", vertices[iter->first]);
        for(int i = 0; i < nb.size(); ++i){
            printf(" %d", vertices[nb[i]]);
        }
        printf("\n");
    }

    map<int, int>::iterator iter2;
    for(iter2 = vertices.begin(); iter2 != vertices.end(); iter2++){
        if(graph.find(iter2->first) == graph.end()){
            printf("%d\n", iter2->second);
        }
    }

    return 0;
}
