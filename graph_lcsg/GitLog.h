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