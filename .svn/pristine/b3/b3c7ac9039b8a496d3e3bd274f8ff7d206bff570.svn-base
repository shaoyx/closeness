#include "Bfs.h"

#define MAXLINE 50000
#define MAXTERM 100
#define THREADN 10
#define MAXDISTANCE (5)
#define phi (0.77351)
#define NMAP 128

#define EPSILON 1e-6

#define MIXFRAMEWORK

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


/*double abs(double t) {
    if (t < 0) return -t;
    return t;
}*/

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

void btopkrank(int k, vector<int> *person_graph, int num_person, int total_num_person, priority_queue <ValuePair> &ans);
void bcyclic_calculation(int qid, set<vertex_centrality>& answer, int k, vector<vertex_degree>& cc_vertices, int cc_size, 
        vector <int> * person_graph, int num_person, int total_num_person);
void bupdate_answer(set<vertex_centrality>& answer, int k, double centality, long vid);
void bbfs(int start, int* estimated_distances, vector <int> * person_graph, int total_num_person, int& estimated_radius);

void Bfs::run(int k) {
    priority_queue<ValuePair> ans;

    int n = maxVertexId;
    vector <int> *vedge = new vector <int>[n + 1]; // edge list

    for (int i = 0; i < n; ++i)
        for (int j = graphSrc[i]; j < graphSrc[i + 1]; ++j)
            vedge[i].push_back(graphDst[j]);

    btopkrank(k, vedge, n, n, ans);

    results.clear();
    while(!ans.empty()){
        ValuePair vp = ans.top();
        results.push_back(vp);
        ans.pop();
    }

    delete[] vedge;
}

vector<ValuePair> Bfs::getResults() {
    return results;
}


/**
 * Process each connected component one by one.
 * 
 * In each connected component, using cyclic select-test method.
 */
void btopkrank(int k, vector<int> *person_graph, int num_person, int total_num_person, priority_queue <ValuePair> &ans) {
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

        /* execute K. Okamoto Algorithms */
        if(cc_size > 1)
            bcyclic_calculation(qid, answers, k, cc_vertices, cc_size, person_graph, num_person, total_num_person);
        else if(cc_size == 1) {
            bupdate_answer(answers, k, 0, cc_vertices[0].id);
        }
    }
    delete [] visitedl;

    /* output the answer. */
    char result[MAXLINE];
    char tmp[MAXTERM];
    sprintf(result, "%d", qid);
    for (set <vertex_centrality>::iterator iter = answers.begin(); iter != answers.end(); ++iter) {
        sprintf(tmp, " %ld", (*iter).id);
        strcat(result, tmp);
        ans.push(ValuePair((*iter).id, (*iter).closeness));
    }

    //printf("%s\n", result);
}


/**
 * cyclic_calculation
 * 
 * Select some candidates by approximate method, calculate the exact centraliry value and update the top-k answers.
 * If the top-k answer set changed, do it again!
 */
void bcyclic_calculation(int qid, set<vertex_centrality>& answers, int k, vector<vertex_degree>& cc_vertices, int cc_size, vector<int> *person_graph, int num_person, int total_num_person) {
    int *exact2hop = new int[total_num_person + 1];

    int num_samples = cc_size;
    //printf("%d\n", num_samples);
    int topke = 14 * k; // choose top 100*k of estimated closeness
    //if (total_num_person > 600000 && k < 5) topke = 1000;
    int topd = 0; // choose top 100 biggest of degree
    double alpha = 1; /* tunning later. */

    vector<vertex_avgdist> estimated_avgdist;
    vector<int> candidates;
    int* estimated_distances = new int[total_num_person+1];
    bool* is_in_candidates = new bool[total_num_person+1];
    int estimated_radius = -1; /**/
    memset(estimated_distances, 0, sizeof(int)*(total_num_person+1));
    memset(is_in_candidates, 0, sizeof(bool)*(total_num_person+1));

    /* using heuristic rules to select seeds and estimate the centrality! */
    for(int i = 0; i < num_samples; ++i) {
        int start = cc_vertices[i].id;
        int tmp_radius = -1;
        bbfs(start, estimated_distances, person_graph, total_num_person, tmp_radius);
        if(tmp_radius < estimated_radius || estimated_radius == -1)
            estimated_radius = tmp_radius;
    }


    delete [] is_in_candidates;
    //estimated_distances = NULL;
    is_in_candidates = NULL;

    /* find the top-k in this connected component and merged into answer. */
    short is_changed = 0;
    //int inc = 50*k;

    for(int i = 0; i < cc_size; ++i) {
        int vid = cc_vertices[i].id;
        double avgdist = estimated_distances[vid];

        double centrality = (double)(cc_size - 1) / (avgdist / num_samples) / (num_person - 1);
        answers.insert(vertex_centrality(centrality, vid));
        if (answers.size() > k)
            answers.erase((*answers.rbegin()));

    }
    


    delete [] exact2hop;
    estimated_avgdist.clear();
}

void bupdate_answer(set<vertex_centrality>& answer, int k, double centrality, long vid) {
    answer.insert(vertex_centrality(centrality, vid));
    if (answer.size() > k) 
        answer.erase((*answer.rbegin()));
}

void bbfs(int start, int* estimated_distances, vector <int> * person_graph, int total_num_person, int& estimated_radius) {
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


