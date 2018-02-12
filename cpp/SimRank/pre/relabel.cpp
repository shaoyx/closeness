/*
 * relabel the graph with successive id,
 * and output the relabeled map (e.g., old_id -> new_id).
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

    while(scanf("%d %d",&a,&b) != EOF){
        if(vertices.find(a) == vertices.end()) {vertices[a] = id++;}
        if(vertices.find(b) == vertices.end()) {vertices[b] = id++;}
    }

    map<int, int>::iterator iter2;
    for(iter2 = vertices.begin(); iter2 != vertices.end(); iter2++){
        printf("%d %d\n", iter2->first, iter2->second);
    }

    return 0;
}
