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

template<typename T>
ostream& operator<<(ostream& o, const unordered_set<T> & st)
{
    for (auto elem: st){
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

class graph{
private:
    vector<unordered_set<int>> adjacency_list;
    int n;
    int m;

    static void combinations(const int n, vector<unordered_set<int>>& list_combinations, int cur = 0, unordered_set<int> currentCombination = {}){
        dbg(cur, n);

        if (cur == n){
            dbg(currentCombination);
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

    void add_edge(const int u, const int v){
        m++;
        adjacency_list[u].insert(v);
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
        dbg(subgraph);
        for (int i = 0; i < n; ++i){
            if (!vertexes.count(i)){
                const unordered_set<int>& cur_adjacents = adjacents(i);
                dbg(cur_adjacents);

                for (int u = 0; u < n; ++u){
                    dbg(u);
                    if (adjacents(u).count(i)){
                        for (auto v: cur_adjacents){
                            subgraph.add_edge(u, v);
                        }
                    }
                }
            }
        }

        dbg(subgraph);

        return subgraph;
    }

    static bool equalSubgraphs(const graph& firstGraph, const graph& secondGraph, unordered_set<int>& vertexes){
        graph firstSubgraph = firstGraph.subgraph(vertexes);
        graph secondSubgraph = secondGraph.subgraph(vertexes);

        for (auto u: vertexes){
            if (firstGraph.adjacents(u) != secondGraph.adjacents(u)){
                return false;
            }
        }

        return true;
    }

    static unordered_set<int> bruteForceGreatestCommonSubgraph(const graph& firstGraph, const graph& secondGraph){
        unordered_set<int> res;

        vector<unordered_set<int>> allCombinations;

        combinations(min(firstGraph.get_n(), secondGraph.get_n()), allCombinations);

        dbg(allCombinations);

        for (auto& combination: allCombinations){
            dbg(combination);
            if (equalSubgraphs(firstGraph, secondGraph, combination) && res.size() < combination.size()){
                res = combination;
            }
        }

        return res;
    }
};

int main(){
    graph firstGraph;
    cin >> firstGraph;
    graph secondGraph;
    cin >> secondGraph;

    cout << firstGraph << endl << secondGraph << endl;

    cout << graph::bruteForceGreatestCommonSubgraph(firstGraph, secondGraph);
    return 0;
}
/*
3 2
1 2
2 3
3 2
1 2
2 3
*/
