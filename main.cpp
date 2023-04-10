#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class graph{
    vector<unordered_set<int>> adjacency_list;
    int n;
    int m;

    friend istream& operator>>(istream& o, graph& cur)
    {
        for (long long i = 0; i < cur.m; ++i){
            long long u, v;
            o >> u; u--;
            o >> v; v--;
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
};

int main(){

    return 0;
}
