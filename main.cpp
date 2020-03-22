#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

struct Nod
{
    Nod()
    {
        start  = false;
        ending = false;
        links  = map<char, vector<int> >();
        nextNodes = vector<pair<int, char> >();
    }

    bool                     start;
    bool                     ending;
    map<char, vector<int> >  links;
    vector<pair<int, char> > nextNodes;
};

bool ValidWord(string       word,
               int          letterIndex,
               int          crtNode,
               vector<Nod>& automat)
{
    // inainte sa realizez ca se poate face si in complexitate polinomiala
    /*
    bool result = false;

    if (letterIndex >= word.size())
    {
        if (automat[crtNode].ending)
            return true;
        return false;
    }

    char c = word[letterIndex];

    if (automat[crtNode].links.find(c) != automat[crtNode].links.end())
    {
        for (int i = 0; i < automat[crtNode].links[c].size(); i++)
            result = result || ValidWord(word,
                                         letterIndex + 1,
                                         automat[crtNode].links[c][i],
                                         automat);
    }
    */

    // primul e indexul caracterului, al doilea e nodul
    queue<pair<int, int> > indexQueue;
    map<pair<int, int>, bool> beenThere;
    bool result = false;

    indexQueue.push(make_pair(0, crtNode));
    beenThere[make_pair(0, crtNode)] = true;

    while (!(indexQueue.empty() || result))
    {
        pair<int, int> cerElem = indexQueue.front();
        indexQueue.pop();

        int strIx = cerElem.first;
        int nod = cerElem.second;

        if (strIx >= word.size())
        {
            if (automat[nod].ending)
                result = true;
        }
        else
        {
            char c = word[strIx];
            for (int i = 0; i < automat[nod].links[c].size(); i++)
            {
                pair<int, int> newPair = make_pair(strIx + 1, automat[nod].links[c][i]);
                if (beenThere.find(newPair) == beenThere.end())
                {
                    beenThere[newPair] = true;
                    indexQueue.push(newPair);
                }
            }
        }
    }

    return result;
}

void BFS(vector<Nod>& automat, int q0)
{
    queue<pair<int, string> > crtWords;
    map<pair<int, string>, bool> beenThere;
    int counter = 0;

    crtWords.push(make_pair(q0, string()));
    beenThere[make_pair(q0, string())] = true;

    while (!(crtWords.empty() || counter >= 100))
    {
        pair<int, string> crtElem = crtWords.front();
        crtWords.pop();

        if (automat[crtElem.first].ending)
        {
            cout << crtElem.second << endl;
            counter++;
        }
        for (int i = 0; i < automat[crtElem.first].nextNodes.size(); i++)
        {
            pair<int, string> newElem = make_pair(automat[crtElem.first].nextNodes[i].first,
                                                  crtElem.second + automat[crtElem.first].nextNodes[i].second);

            if (beenThere.find(newElem) == beenThere.end())
            {
                beenThere[newElem] = true;
                crtWords.push(newElem);
            }
        }
    }
}

int main()
{
    ifstream fin("automat.in");
    vector<Nod> automat;

    int n, m;
    fin >> n >> m;
    automat.resize(n);

    for (int i = 0; i < m; i++)
    {
        int q1, q2;
        char c;
        fin >> q1 >> q2 >> c;
        if (automat[q1].links.find(c) == automat[q1].links.end())
            automat[q1].links[c] = vector<int>();
        automat[q1].links[c].push_back(q2);
        automat[q1].nextNodes.push_back(make_pair(q2, c));
    }

    int q0, nf;
    fin >> q0 >> nf;

    automat[q0].start = true;

    for (int i = 0; i < nf; i++)
    {
        int qf;
        fin >> qf;
        automat[qf].ending = true;
    }

    string str;
    cin >> str;

    cout << endl;

    BFS(automat, 0);

    cout << endl;

    cout << (ValidWord(str,
                       0,
                       q0,
                       automat) ? "Da man, e valid cuvantul" : "Nope, cuvantul nu este valid..")
         << endl;


    fin.close();

    return 0;
}
