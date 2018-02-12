#include "spt.h"

void SPT::run(int k){
    priority_queue<ValuePair> ans;
    int* seed = new int[k];
    double* cc_bound = new double[maxVertexId];
    AdaptiveHeap* boundHeap = new AdaptiveHeap();
    
    int* vis = new int[maxVertexId];
    int ts = 0;
    int* dist = new int[maxVertexId];
    int* visSeq = new int[maxVertexId];
    
    memset(vis, 0, sizeof(vis));
    for(int i = 0; i < maxVertexId; ++i){
        cc_bound[i] = -1;
    }
    
    //1. select k vertices as starting points.
    select_k_vertices(k, seed);

    //printf("seeds: ");
    //printArray(seed, k);
    
    //2. compute k's shortest path tree
    for(int i = 0; i < k; ++i){
        memset(dist, -1, sizeof(dist));
        ++ts;
        int ccSize = 0;
        int spSum = bfs(seed[i], vis, ts, dist, visSeq, ccSize);
        double cc = 0.0;
        if(spSum > 0 && maxVertexId > 1){
            cc = (ccSize- 1.0) / (maxVertexId - 1.0) * (ccSize - 1.0) / spSum;
        }
        ans.push(ValuePair(seed[i], cc));
        cc_bound[seed[i]] = 0.0;
        computeSpBound(seed[i], spSum, dist, visSeq, ccSize, cc_bound);
        /*printf("dist: ");printArray(dist, maxVertexId);
        printf("visSeq: ");printArray(visSeq, ccSize);
        printf("ccbound: ");
        printArray(cc_bound, maxVertexId);
        printf("\n");*/
    }
    
    boundHeap->buildHeap(cc_bound, maxVertexId, maxVertexId);
    //3. select the next vertex from min-heap and do the update
    int tot = k;
    while(boundHeap->isEmpty() == false){
        element ele = boundHeap->pop();
        tot++;
        //use filter here.
        if(ans.top().getValue() > ele.weight){ //the previous steps ensure that ans has k elements.
            break;
        }
        memset(dist, -1, sizeof(dist));
        ++ts;
        int ccSize = 0;
        int spSum = bfs(ele.id, vis, ts, dist, visSeq, ccSize);
        double cc = 0.0;
        if(spSum > 0 && maxVertexId > 1){
            cc = (ccSize- 1.0) / (maxVertexId - 1.0) * (ccSize - 1.0) / spSum;
        }
        ans.push(ValuePair(ele.id, cc));
        ans.pop();
        cc_bound[ele.id] = 0.0;
        updateSpBound(ele.id, spSum, dist, visSeq, ccSize, cc_bound, boundHeap);
    }

    //printf("total cal=%d\n", tot);
    
    //4. output the results.
    results.clear();
    while(!ans.empty()){
        ValuePair vp = ans.top();
        results.push_back(vp);
        ans.pop();
    }
    delete [] seed;
    delete [] cc_bound;
    delete boundHeap;
    delete [] visSeq;
}

int SPT::bfs(int s, int* vis, int ts, int* dist, int* visSeq, int& ccSize){
    int res = 0;
    ccSize = 0;
    queue<int> q;
    q.push(s);
    vis[s] = ts;
    dist[s] = 0;
    while(!q.empty()){
        int cur = q.front();
        q.pop();
        visSeq[ccSize] = cur;
        ccSize++;
        int s = graphSrc[cur], e = graphSrc[cur+1];
        for(int i = s; i < e; ++i){
            if(vis[graphDst[i]] != ts){
                q.push(graphDst[i]);
                vis[graphDst[i]] = ts;
                dist[graphDst[i]] = dist[cur] + 1;
                res += dist[graphDst[i]];
            }
        }
    }
    return res;
}

void SPT::computeSpBound(int sv, int spSum, int* dist, int* visSeq, int ccSize, double* cc_bound){
    if(spSum == 0) return;
    int left = 1, right = ccSize - 1;
    int sum = spSum;
    for(int i = 1; i < ccSize; i++){
        left++;
        right--;
        sum += (dist[visSeq[i]] - dist[visSeq[i-1]]) * (left - right - 2);
        //printf("(%d %d)\n", visSeq[i], sum);
        double nb = (ccSize - 1.0) / (maxVertexId - 1.0) * (ccSize - 1.0) / sum;
        if(cc_bound[visSeq[i]] < 0 || nb < cc_bound[visSeq[i]])
            cc_bound[visSeq[i]] = nb;
    }
}

void SPT::updateSpBound(int sv, int spSum, int* dist, int* visSeq, int ccSize, double* cc_bound, AdaptiveHeap* heap){
    if(spSum == 0) return;
    int left = 1, right = ccSize - 1;
    int sum = spSum;
    for(int i = 1; i < ccSize; i++){
        left++;
        right--;
        sum += (dist[visSeq[i]] - dist[visSeq[i-1]]) * (left - right - 2);
        double nb = (ccSize - 1.0) / (maxVertexId-1.0) * (ccSize - 1.0) / sum;
        if(cc_bound[visSeq[i]] < 0 || nb < cc_bound[visSeq[i]]){
            cc_bound[visSeq[i]] = nb;
            heap->update(visSeq[i], nb);
        }
    }
}

void SPT::select_k_vertices(int k, int* seed){
    Random rand;
    map<int, bool> exist;
    int i = 0;
    while(i < k){
        int rnd = rand.getRandom() % maxVertexId;
        if(rnd < 0){
            rnd = (rnd + maxVertexId) % maxVertexId;
        }
        if(exist.find(rnd) == exist.end()){
            exist[rnd] = true;
            seed[i] = rnd;
            ++i;
        }
    }
    exist.clear();
}

void SPT::printArray(int* array, int size){
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

void SPT::printArray(double* array, int size){
    for(int i = 0; i < size; i++){
        printf("%.5lf ", array[i]);
    }
    printf("\n");
}

vector<ValuePair> SPT::getResults(){
    return results;
}
