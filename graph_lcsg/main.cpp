#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

#ifdef MATVEY
#define _dbg(x) do { cout << #x << "=" << x << "; "; } while (0)
#define _name(name, _1, _2, _3, _4, N, ...) name ## N
#define _dbg1(x) _dbg(x)
#define _dbg2(x, ...) _dbg(x); _dbg1(__VA_ARGS__)
#define _dbg3(x, ...) _dbg(x); _dbg2(__VA_ARGS__)
#define _dbg4(x, ...) _dbg(x); _dbg3(__VA_ARGS__)
#define dbg(...) do { cout << __LINE__ << ": "; _name(_dbg, __VA_ARGS__, 4, 3, 2, 1, 0)(__VA_ARGS__); cout << endl;} while (0)
#else
#define dbg(...)
#endif
#include <bits/stdc++.h>

using namespace std;

#ifdef MATVEY
#define _dbg(x) do { cout << #x << "=" << x << "; "; } while (0)
#define _name(name, _1, _2, _3, _4, N, ...) name ## N
#define _dbg1(x) _dbg(x)
#define _dbg2(x, ...) _dbg(x); _dbg1(__VA_ARGS__)
#define _dbg3(x, ...) _dbg(x); _dbg2(__VA_ARGS__)
#define _dbg4(x, ...) _dbg(x); _dbg3(__VA_ARGS__)
#define dbg(...) do { cout << __LINE__ << ": "; _name(_dbg, __VA_ARGS__, 4, 3, 2, 1, 0)(__VA_ARGS__); cout << endl;} while (0)
#else
#define dbg(...)
#endif

template<typename X, typename Y>
ostream& operator<<(ostream& o, const pair<X, Y>& p)
{
    return o << "<" << p.first << ", " << p.second << ">" << "\n";
}

template<typename X, typename Y>
ostream& operator<<(ostream& o, const map<X, Y>& mp)
{
    for (auto elem: mp){
        o << elem << " ";
    }
    o << endl;
    return o;
}

template<typename T>
ostream& operator<<(ostream& o, const vector<T> & st)
{
    for (auto elem: st){
        o << elem << " ";
    }
    o << endl;
    return o;
}

template<typename T>
ostream& operator<<(ostream& o, const unordered_set<T> & st)
{
    for (auto elem: st){
        o << elem << " ";
    }
    o << endl;
    return o;
}

class graph{
private:
    vector<unordered_set<int>> adjacency_list;
    int n;
    int m;

    static void combinations(const int n, vector<unordered_set<int>>& list_combinations, int cur = 0, unordered_set<int> currentCombination = {}){

        if (cur == n){
            list_combinations.push_back(currentCombination);
            return;
        }

        combinations(n, list_combinations, cur + 1, currentCombination);

        currentCombination.insert(cur);

        combinations(n, list_combinations, cur + 1, currentCombination);

        currentCombination.erase(cur);

        return;
    }

public:
    int get_n() const{
        return n;
    }

    graph(){
    }

    graph(const int n){
        this->n = n;
    }

    graph(const vector<pair<int, int>>& listOfEdges){
        int mx = 0;
        for (auto pr: listOfEdges){
            mx = max({mx, pr.second, pr.first});
        }

        n = mx + 1;

        adjacency_list.resize(n);

        for (auto pr: listOfEdges){
            add_edge(pr.first, pr.second);
        }
    }

    void add_edge(const int u, const int v){
        m++;
        adjacency_list[u].insert(v);
    }

    void remove_edge(const int u, const int v){
        m--;
        adjacency_list[u].erase(v);
    }

    const unordered_set<int>& adjacents(const int u) const{
        return adjacency_list[u];
    }

    friend istream& operator>>(istream& o, graph& cur){
        o >> cur.n >> cur.m;
        cur.adjacency_list.resize(cur.n);
        for (long long i = 0; i < cur.m; ++i){
            long long u, v;
            o >> u;
            o >> v;
            cur.adjacency_list[u].insert(v);
        }
        return o;
    }

    friend ostream& operator<<(ostream& o, const graph& cur){
        o << "Vertexes: " << cur.n << "\n";
        o << "Edges: " << cur.m << "\n";

        for (long long u = 0; u < cur.n; ++u){
            o << u << ":";
            o << '(';
            for (auto v: cur.adjacency_list[u]){
                o << v;
                o << ',';
            }
            o << ')';
            o << '\n';
        }
        return o;
    }


    bool achievable(const int from, const int to) const{
        static vector<int> visited(n);

        if (to == from){
            return true;
        }

        if (visited[from]){
            return false;
        }

        visited[from] = true;

        bool res = 0;
        for (auto v: adjacency_list[from]){
            res = res || achievable(v, to);
        }

        return res;
    }

    graph subgraph(const unordered_set<int>& vertexes) const{
        graph subgraph = *this;
        for (int i = 0; i < n; ++i){
            if (!vertexes.count(i)){
                const unordered_set<int>& cur_adjacents = adjacents(i);

                for (int u = 0; u < n; ++u){
                    if (adjacents(u).count(i)){
                        for (auto v: cur_adjacents){
                            subgraph.add_edge(u, v);
                        }
                    }
                }
            }
        }

        for (int i = 0; i < n; ++i){
            if (!vertexes.count(i)){
                for (int u = 0; u < n; ++u){
                    if (adjacents(u).count(i)){
                        subgraph.remove_edge(u, i);
                    }
                }
            }
        }

        return subgraph;
    }

    static bool equalSubgraphs(const graph& firstGraph, const graph& secondGraph, unordered_set<int>& vertexes){
        graph firstSubgraph = firstGraph.subgraph(vertexes);
        graph secondSubgraph = secondGraph.subgraph(vertexes);

        unordered_set<int> un_st = {7, 1, 0, 2, 4, 5, 6};

        for (auto u: vertexes){
            if (firstSubgraph.adjacents(u) != secondSubgraph.adjacents(u)){
                return false;
            }
        }

        return true;
    }

    static unordered_set<int> bruteForceGreatestCommonSubgraph(const graph& firstGraph, const graph& secondGraph){
        unordered_set<int> res;

        vector<unordered_set<int>> allCombinations;

        combinations(min(firstGraph.get_n(), secondGraph.get_n()), allCombinations);

        for (auto& combination: allCombinations){
            if (equalSubgraphs(firstGraph, secondGraph, combination) && res.size() < combination.size()){
                res = combination;
            }
        }

        return res;
    }

    static unordered_set<int> cartesianProductGreatestCommonSubgraph(const graph& firstGraph, const graph& secondGraph){
        int maxVertex = min(firstGraph.get_n(), secondGraph.get_n());

        vector<vector<unordered_set<int>>> result(maxVertex, vector<unordered_set<int>>(maxVertex));

        result[0][0] = {0};

        for (int addedVertex = 0; addedVertex < maxVertex; ++addedVertex){
            for (int predVertexFirstGraph = 0; predVertexFirstGraph < addedVertex; ++predVertexFirstGraph){
                for (int predVertexSecondGraph = 0; predVertexSecondGraph < addedVertex; ++predVertexSecondGraph){
                    unordered_set<int> current = result[predVertexFirstGraph][predVertexSecondGraph];
                    current.insert(addedVertex);
                    if (equalSubgraphs(firstGraph, secondGraph, current) && result[addedVertex][addedVertex].size() < current.size()){
                        result[addedVertex][addedVertex] = current;
                    }
                }
            }
        }

        return result[maxVertex - 1][maxVertex - 1];
    }
};

class GitLog{
private:
    vector<string> gitLog;
    string directoryName;

    void generateListOfEdgesFromGitLog(int str, int stb, const string& curHash, const vector<string>& gitLog, set<tuple<string, int, int>>& visited, vector<pair<string, string>>& listOfEdges) const{
        if (visited.count({curHash, str, stb})){
            return;
        }

        visited.insert({curHash, str, stb});

        if (gitLog[str][stb] == '*'){
            string newHash = gitLog[str].substr(gitLog[str].find("commit") + 7);
            if (curHash != ""){
                listOfEdges.push_back({curHash, newHash});
            }
            generateListOfEdgesFromGitLog(str + 1, stb, newHash, gitLog, visited, listOfEdges);
            return;
        }

        if (gitLog[str][stb] == '|'){
            generateListOfEdgesFromGitLog(str + 1, stb, curHash, gitLog, visited, listOfEdges);
        }
        if (gitLog[str][stb + 1] == '\\'){
            generateListOfEdgesFromGitLog(str + 1, stb + 2, curHash, gitLog, visited, listOfEdges);
        }
        if (stb >= 2 && gitLog[str][stb - 1] == '/'){
            generateListOfEdgesFromGitLog(str + 1, stb - 2, curHash, gitLog, visited, listOfEdges);
        }
    }

public:
    GitLog(const string& directoryName){
        this->directoryName = directoryName;
        string queryLog = "cd " + directoryName + " && git log --graph > gitlog.txt";

        system(queryLog.c_str());

        ifstream input(directoryName + "\\gitlog.txt");

        string line;

        while (getline(input, line)){
            gitLog.push_back(line);
        }
    }

    vector<pair<string, string>> generateListOfEdges() const{
        set<tuple<string, int, int>> visited;
        vector<pair<string, string>> listOfEdges;

        generateListOfEdgesFromGitLog(0, 0, "", gitLog, visited, listOfEdges);

        return listOfEdges;
    }

    map<string, vector<string>> generateDiffForEveryHash() const{
        vector<pair<string, string>> listOfEdges = generateListOfEdges();

        set<string> hashes;

        for (auto elem: listOfEdges){
            hashes.insert(elem.first);
            hashes.insert(elem.second);
        }

        map<string, vector<string>> res;

        for (auto elem: hashes){
            string queryLog = "cd " + directoryName + " && git diff " + elem + " > gitlog.txt";
            system(queryLog.c_str());

            vector<string> diff;

            ifstream input(directoryName + "\\gitlog.txt");

            string line;

            while (getline(input, line)){
                diff.push_back(line);
            }

            res[elem] = diff;
        }

        return res;
    }
};

vector<pair<vector<string>, vector<string>>> createlistOfEdgesDiffFromLinkOnRep(const string& linkOnRep){
    GitLog gitLog("C:\\Users\\ochob\\OneDrive\\������� ����\\6 �������\\mercurial\\lab6\\laba\\mylab6");

    map<string, vector<string>> diff = gitLog.generateDiffForEveryHash();

    vector<pair<string, string>> listOfEdges = gitLog.generateListOfEdges();

    vector<pair<vector<string>, vector<string>>> listOfEdgesDiff;

    for (int i = 0; i < listOfEdges.size(); ++i){
        listOfEdgesDiff.push_back({diff[listOfEdges[i].first], diff[listOfEdges[i].second]});
    }

    return listOfEdgesDiff;
}

map<vector<string>, int> numerateDiff(const vector<pair<vector<string>, vector<string>>>& firstListOfEdges, const vector<pair<vector<string>, vector<string>>>& secondListOfEdges){
    map<vector<string>, int> res;
    int cnt = -1;

    for (int i = 0; i < firstListOfEdges.size(); ++i){
        if (res.find(firstListOfEdges[i].first) == res.end()){
            res[firstListOfEdges[i].first] = ++cnt;
        }
        if (res.find(firstListOfEdges[i].second) == res.end()){
            res[firstListOfEdges[i].second] = ++cnt;
        }
    }

    for (int i = 0; i < secondListOfEdges.size(); ++i){
        if (res.find(secondListOfEdges[i].first) == res.end()){
            res[secondListOfEdges[i].first] = ++cnt;
        }
        if (res.find(secondListOfEdges[i].second) == res.end()){
            res[secondListOfEdges[i].second] = ++cnt;
        }
    }

    return res;
}

vector<pair<int, int>> madeListOfEdgesFromListOfEdgesDiff(const vector<pair<vector<string>, vector<string>>>& listOfEdgesDiff, map<vector<string>, int>& numberOfDiff){
    vector<pair<int, int>> res;
    for (int i = 0; i < listOfEdgesDiff.size(); ++i){
        res.push_back({numberOfDiff[listOfEdgesDiff[i].first], numberOfDiff[listOfEdgesDiff[i].second]});
    }
    return res;
}

tuple<graph, graph, map<vector<string>, int>> generateGraphsFromGitLinks(const string& link1, const string& link2){
    vector<pair<vector<string>, vector<string>>> listOfEdgesDiff1 = createlistOfEdgesDiffFromLinkOnRep("C:\\Users\\ochob\\OneDrive\\������� ����\\6 �������\\mercurial\\lab6\\laba\\mylab6");
    vector<pair<vector<string>, vector<string>>> listOfEdgesDiff2 = createlistOfEdgesDiffFromLinkOnRep("C:\\Users\\ochob\\OneDrive\\������� ����\\6 �������\\mercurial\\lab6\\laba\\mylab6");

    map<vector<string>, int> numberOfDiff = numerateDiff(listOfEdgesDiff1, listOfEdgesDiff2);

    //dbg(numberOfDiff);

    vector<pair<int, int>> listOfEdges1 = madeListOfEdgesFromListOfEdgesDiff(listOfEdgesDiff1, numberOfDiff);
    vector<pair<int, int>> listOfEdges2 = madeListOfEdgesFromListOfEdgesDiff(listOfEdgesDiff2, numberOfDiff);

    dbg(listOfEdges1);
    //dbg
    graph graph1(listOfEdges1);
    graph graph2(listOfEdges2);

    return {graph1, graph2, numberOfDiff};
}

int main()
{
    tuple<graph, graph, map<vector<string>, int>> graphs = generateGraphsFromGitLinks("C:\\Users\\ochob\\OneDrive\\������� ����\\6 �������\\mercurial\\lab6\\laba\\mylab6", "C:\\Users\\ochob\\OneDrive\\������� ����\\6 �������\\mercurial\\lab6\\laba\\mylab6");

    dbg(get<0>(graphs));
    dbg(get<1>(graphs));

    unordered_set<int> resGraph = graph::bruteForceGreatestCommonSubgraph(get<0>(graphs), get<1>(graphs));
    dbg(resGraph);
    return 0;
}

/*
8 7
1 2
2 3
3 4
4 5
5 6
6 7
3 5
8 7
1 2
2 3
3 4
4 5
5 6
6 7
2 5
*/