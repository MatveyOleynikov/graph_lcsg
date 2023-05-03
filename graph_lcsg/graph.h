#include <bits/stdc++.h>

using namespace std;

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
