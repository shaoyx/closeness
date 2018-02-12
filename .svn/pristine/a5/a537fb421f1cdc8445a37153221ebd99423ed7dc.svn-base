#include "DeltaPFS.h"

#define MAXLINE 1000000
#define MAXTERM 100
#define THREADN 10
#define MAXDISTANCE (5)
#define phi (0.77351)
#define NMAP 128

#define EPSILON 1e-6

/**
 * record the closeness cetrality of vertex id
 */
class vertex_centrality {
    public:
        long id;
        double closeness;
        vertex_centrality() { }
        vertex_centrality(double closeness_, int id_):id(id_), closeness(closeness_) { }

};


/**
 *  * record the average distance of vertex id
 *   */
class vertex_avgdist {
    public:
        int id;
        double avgdist;
        vertex_avgdist() { }
        vertex_avgdist(double avgdist_, int id_):id(id_), avgdist(avgdist_) { }

};

/**
 * record the vertex id and its degree
 */
class vertex_degree {
    public:
        int id;
        int degree;
        vertex_degree() { }
        vertex_degree(int degree_, int id_):id(id_), degree(degree_) { }

};


double abs(double t) {
    if (t < 0) return -t;
    return t;
}

inline bool operator< (const vertex_centrality &la, const vertex_centrality &lb) {
    if (abs(la.closeness - lb.closeness) > EPSILON) 
        return la.closeness > lb.closeness;
    return la.id < lb.id;

}

inline bool operator< (const vertex_avgdist &la, const vertex_avgdist &lb) {
    if (abs(la.avgdist - lb.avgdist) > EPSILON) 
        return la.avgdist < lb.avgdist;
    return la.id < lb.id;

}

inline bool operator< (const vertex_degree &la, const vertex_degree &lb) {
    if (la.degree != lb.degree) 
        return la.degree < lb.degree;
    return la.id < lb.id;

}

void topkrank(int k, vector<int> *person_graph, int num_person, int total_num_person, priority_queue <ValuePair> &ans);
void cyclic_calculation(int qid, set<vertex_centrality>& answer, int k, vector<vertex_degree>& cc_vertices, int cc_size, 
        vector <int> * person_graph, int num_person, int total_num_person);
void update_answer(set<vertex_centrality>& answer, int k, double centality, long vid);
void bfs(int start, int* estimated_distances, vector <int> * person_graph, int total_num_person, int& estimated_radius);
void delta_pfs(set <vertex_centrality> &answer, int k, vector <vertex_degree> &vp, vector <int> *vedge, int total_num_person, int cc_size, int num_person, short& is_changed);


void DeltaPFS::run(int k) {
    priority_queue<ValuePair> ans;

    int n = maxVertexId;
    vector <int> *vedge = new vector <int>[n + 1]; // edge list

    for (int i = 0; i < n; ++i)
        for (int j = graphSrc[i]; j < graphSrc[i + 1]; ++j)
            vedge[i].push_back(graphDst[j]);

    topkrank(k, vedge, n, n, ans);

    results.clear();
    while(!ans.empty()){
        ValuePair vp = ans.top();
        results.push_back(vp);
        ans.pop();
    }

    delete[] vedge;
}

vector<ValuePair> DeltaPFS::getResults() {
    return results;
}

inline unsigned int popcnt( unsigned int x ) {
    x -= ((x >> 1) & 0x55555555);
    x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
    x = (((x >> 4) + x) & 0x0f0f0f0f);
    x += (x >> 8);
    x += (x >> 16);
    return x & 0x0000003f;
}

/**
 * Process each connected component one by one.
 * 
 * In each connected component, using cyclic select-test method.
 */
void topkrank(int k, vector<int> *person_graph, int num_person, int total_num_person, priority_queue <ValuePair> &ans) {
    int qid = 0;
    bool *visitedl = new bool[total_num_person + 1];
    vector<vertex_degree> cc_vertices;
    int cc_size = 0;
    queue <int> search_queue;
    memset(visitedl, 0, sizeof(bool) * (total_num_person + 1));
    set<vertex_centrality> answers;

    for (int idx = 0; idx < num_person; ++idx) {
        int start = idx;
        if (visitedl[start]) 
            continue;

        /* find one component using BFS. */
        search_queue.push(start);
        visitedl[start] = 1;
        cc_vertices.clear();
        cc_size = 0;
        while (!search_queue.empty()) {
            int cur = search_queue.front();
            search_queue.pop();
            cc_vertices.push_back(vertex_degree(person_graph[cur].size(), cur));
            ++cc_size;
            for (int nbr = 0; nbr < person_graph[cur].size(); ++nbr) {
                int nv = person_graph[cur][nbr];
                if (visitedl[nv])
                    continue;
                visitedl[nv] = 1;
                search_queue.push(nv);
            }
        }

        /* sort the vertices by the degree in ascending order.*/
        sort(cc_vertices.begin(), cc_vertices.end());

        short is_changed = 0; // just fill the arguments, no meaning
        /* execute K. Okamoto Algorithms */
        if(cc_size > 1) 
            delta_pfs(answers, k, cc_vertices, person_graph, total_num_person, cc_size, num_person, is_changed);
        else if(cc_size == 1) {
            update_answer(answers, k, 0, cc_vertices[0].id);
        }
        //printf("%d\n", cc_size);
    }
    delete [] visitedl;

    /* output the answer. */
    char result[MAXLINE];
    char tmp[MAXTERM];
    //sprintf(result, "%d", qid);
    for (set <vertex_centrality>::iterator iter = answers.begin(); iter != answers.end(); ++iter) {
        //sprintf(tmp, " %ld", (*iter).id);
        //strcat(result, tmp);
        ans.push(ValuePair((*iter).id, (*iter).closeness));
    }

    //printf("%s\n", result);
}

/** 
 * Process procedure of delta_pfs.
 *
 */
void process(int i, int *dist, int sumDist, set <vertex_centrality> &answer, int *father, int k, vector <int> *vedge, int sumN, int count, bool *prunable, int &total, int level, short& is_changed) {
    level++;
    double old_centrality;

    if (sumN > 1) old_centrality = (double)(sumN - 1) * (sumN - 1) / sumDist / (count - 1);
    else old_centrality = 0;

    int n = count;
    bool *v = new bool[n + 1];
    int *lambda = new int[n + 1];
    int beginT = clock();
    memmove(lambda, dist, sizeof(int) * (n + 1));
    int old_s = sumDist;
    for (int y = 0; y < vedge[i].size(); ++y) {
        int o = vedge[i][y];
        if (father[o] != i)
            continue;

        double centrality;
        if (sumN > 1) centrality = (double)(sumN - 1) * (sumN - 1) / (sumDist - sumN + 2) / (count - 1);
        else centrality = 0;
        if (answer.size() >= k && centrality < (answer.rbegin()->closeness - 1e-6) && prunable[o])
            continue;

        total++;

        sumDist += sumN - 2;
        dist[o] = dist[i] - 1;

        queue <int> list;
        memset(v, 0, sizeof(bool) * (n + 1));
        v[o] = 1;
        list.push(o);

        int cnt = 0;
        while (!list.empty()) {
            int cur = list.front();
            v[cur] = 1;
            list.pop();
            for (int t = 0; t < vedge[cur].size(); ++t) {
                int j = vedge[cur][t];
                cnt++;
                if (v[j])
                    continue;
                v[j] = 1;
                if (dist[cur] + 1 >= dist[j])
                    continue;
                list.push(j);
                sumDist -= dist[j] - (dist[cur] + 1);
                dist[j] = dist[cur] + 1;
            }
        }
        if (sumN > 1) centrality = (double)(sumN - 1) * (sumN - 1) / (sumDist) / (count - 1);
        else centrality = 0;
        answer.insert(vertex_centrality(centrality, o));
        if (answer.size() > k) {
            if(is_changed > 0 && answer.rbegin()->id != o) {
                is_changed *= -1;
            }
            answer.erase(--(answer.end()));
        }

        process(o, dist, sumDist, answer, father, k, vedge, sumN, count, prunable, total, level, is_changed);

        memmove(dist, lambda, sizeof(int) * (n + 1));

        sumDist = old_s;
    }
    delete[] lambda;
    delete[] v;
}

/**
 * Calculate exact clossness centraliry value here.
 *
 * Using the methon mentioned in "Efficient Top-k Closeness Centrality Search"
 * ICDE 2014
 */
void delta_pfs(set <vertex_centrality> &answer, int k, vector <vertex_degree> &vp, vector <int> *vedge, int total_num_person, int cc_size, int num_person, short& is_changed) {
    int n = total_num_person;

    bool *vpl = new bool[n + 1];
    bool *visitedl = new bool[n + 1];
    bool *v = new bool[n + 1];
    int *father = new int[n + 1];
    int *dist = new int[n + 1];
    bool *prunable = new bool[n + 1];
    queue <int> list;

    memset(vpl, 0, sizeof(bool) * (n + 1));
    memset(father, -1, sizeof(int) * (n + 1));
    memset(dist, 0, sizeof(int) * (n + 1));
    memset(prunable , 1, sizeof(bool) * (n + 1));

    for (int z = 0; z < vp.size(); ++z)
        vpl[vp[z].id] = 1;
    /*
     * find a schedule and vertice number of each component
     * */
    int total = 0;
    sort(vp.begin(), vp.end());	
    memset(visitedl, 0, sizeof(bool) * (n + 1));
    for (int z = 0; z < vp.size(); ++z) {
        int i = vp[z].id;
        if (visitedl[i]) continue;
        visitedl[i] = 1;
        int sumDist = 0, sumN = 0;		
        double centrality;

        memset(dist, 0, sizeof(int) * (n + 1));
        int maxd = 0;
        queue <int> schedule;
        schedule.push(i);

        /* calc dist map for vertex i */
        memset(v, 0, sizeof(bool) * (n + 1));
        v[i] = 1;
        list.push(i);
        while (!list.empty()) {
            int cur = list.front();
            v[cur] = 1;
            list.pop();
            sumDist += dist[cur];
            for (int t = 0; t < vedge[cur].size(); ++t) {
                int j = vedge[cur][t];
                if (v[j]) 
                    continue;
                v[j] = 1;
                list.push(j);
                dist[j] = dist[cur] + 1;
            }
        }

        /* determine the schedular from vertex i */
        list.push(i);
        while (!list.empty()) {
            int cur = list.front();
            visitedl[cur] = 1;
            list.pop();
            for (int t = 0; t < vedge[cur].size(); ++t) {
                int j = vedge[cur][t];
                if (visitedl[j] || !vpl[j])
                    continue;
                visitedl[j] = 1;
                list.push(j);
                father[j] = cur;
                prunable[cur] = 0;
            }
        }

        if (cc_size > 1) centrality = (double)(cc_size - 1) * (cc_size - 1) / (sumDist) / (num_person - 1);
        else centrality = 0;
        answer.insert(vertex_centrality(centrality, i));
        if (answer.size() > k) {
            if(is_changed > 0 && answer.rbegin()->id != i) {
                is_changed *= -1;
            }
            answer.erase(--(answer.end()));
        }

        /* from now, begin the delta-pfs */
        process(i, dist, sumDist, answer, father, k, vedge, cc_size, num_person, prunable, total, 0, is_changed);
    }
    delete [] vpl;
    delete [] visitedl;
    delete [] v;
    delete [] father;
    delete [] dist;
    delete [] prunable;
}


void update_answer(set<vertex_centrality>& answer, int k, double centrality, long vid) {
    answer.insert(vertex_centrality(centrality, vid));
    if (answer.size() > k) 
        answer.erase((*answer.rbegin()));
}

void bfs(int start, int* estimated_distances, vector <int> * person_graph, int total_num_person, int& estimated_radius) {
    int *dist = new int[total_num_person + 1];
    queue <int> search_queue;
    memset(dist, -1, sizeof(int) * (total_num_person + 1));

    /* compute shortest path using BFS. */
    search_queue.push(start);
    dist[start] = 0;

    while(!search_queue.empty()) {
        int cur = search_queue.front();
        search_queue.pop();
        estimated_distances[cur] += dist[cur];
        if(dist[cur] > estimated_radius)
            estimated_radius = dist[cur];

        for(int nbr = 0; nbr < person_graph[cur].size(); ++nbr) {
            int nv = person_graph[cur][nbr];
            if (dist[nv] != -1)
                continue;
            dist[nv] = dist[cur] + 1;
            search_queue.push(nv);
        }
    }
    delete [] dist;
}


