#include <bits/stdc++.h>
#include "dbg.h"
#include "graph.h"
#include "GitLog.h"

using namespace std;

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

vector<pair<vector<string>, vector<string>>> createlistOfEdgesDiffFromLinkOnRep(const string& linkOnRep){
    GitLog gitLog("C:\\Users\\ochob\\OneDrive\\Рабочий стол\\6 семестр\\mercurial\\lab6\\laba\\mylab6");

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
    vector<pair<vector<string>, vector<string>>> listOfEdgesDiff1 = createlistOfEdgesDiffFromLinkOnRep("C:\\Users\\ochob\\OneDrive\\Рабочий стол\\6 семестр\\mercurial\\lab6\\laba\\mylab6");
    vector<pair<vector<string>, vector<string>>> listOfEdgesDiff2 = createlistOfEdgesDiffFromLinkOnRep("C:\\Users\\ochob\\OneDrive\\Рабочий стол\\6 семестр\\mercurial\\lab6\\laba\\mylab6");

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
    tuple<graph, graph, map<vector<string>, int>> graphs = generateGraphsFromGitLinks("C:\\Users\\ochob\\OneDrive\\Рабочий стол\\6 семестр\\mercurial\\lab6\\laba\\mylab6", "C:\\Users\\ochob\\OneDrive\\Рабочий стол\\6 семестр\\mercurial\\lab6\\laba\\mylab6");

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
