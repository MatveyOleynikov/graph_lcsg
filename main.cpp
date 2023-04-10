#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class graph{
private:
    vector<unordered_set<int>> adjacency_list;
    int n;
    int m;

    static void combinations(const int n, vector<vector<int>>& combinations, int cur = 0, vector<int> currentCombination = {}){
        if (cur == n){
            combinations.push_back(currentCombination);
        }

        combinations(n, combinations, cur + 1, currentCombination);

        currentCombination.push_back(cur);

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

    void add_edge(const int u, const int v){
        m++;
        adjacency_list[u].insert(v);
    }

    unordered_set<int> adjacents(const int u) const{
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

    graph subgraph(const vector<int>& vertexes) const{
        graph subgraph(n);
        for (auto u: vertexes){
            for (auto v: vertexes){
                if (achievable(u, v)){
                    subgraph.add_edge(u, v);
                }
            }
        }

        return subgraph;
    }

    static bool equalSubgraphs(const graph& firstGraph, const graph& secondGraph, vector<int>& vertexes){
        graph firstSubgraph = firstGraph.subgraph(vertexes);
        graph secondSubgraph = secondGraph.subgraph(vertexes);

        for (auto u: vertexes){
            if (firstGraph.adjacents(u) != secondGraph.adjacents(u)){
                return false;
            }
        }

        return true;
    }

    static vector<int> bruteForceGreatestCommonSubgraph(const graph& firstGraph, const graph& secondGraph){
        vector<int> res;

        vector<vector<int>> allCombinations;

        combinations(min(firstGraph.get_n(), secondGraph.get_n()), allCombinations);

        return res;
    }
};

int main(){
    graph firstGraph;
    cin >> firstGraph;
    graph secondGraph;
    cin >> secondGraph;

    cout << firstGraph << "\n" << secondGraph << "\n";
    return 0;
}
