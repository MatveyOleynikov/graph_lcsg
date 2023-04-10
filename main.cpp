#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class graph{
private:
    vector<unordered_set<int>> adjacency_list;
    int n;
    int m;


public:
    friend istream& operator>>(istream& o, graph& cur)
    {
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

    friend ostream& operator<<(ostream& o, const graph& cur)
    {
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


    bool achievable(int from, int to){
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
};

int main(){
    graph firstGraph;
    cin >> firstGraph;
    graph secondGraph;
    cin >> secondGraph;

    cout << firstGraph << "\n" << secondGraph << "\n";
    return 0;
}
