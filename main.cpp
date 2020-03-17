#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
using namespace std;

struct Graph {

    int _numVertex;

    stack<int> _stack; 
    list<int> *_adjLists;

    int  *_lowList;
    int  *_dtimeList;
    int  *_valueList;
    int *_maxValues;
    bool *_instackList;

    public:

    Graph(int i) {
    }

    void setV(int numVertex) {
        _numVertex = numVertex;
        _adjLists = new list<int>[_numVertex];
        _lowList = new int[_numVertex];
        _dtimeList = new int[_numVertex];
        _valueList = new int[_numVertex];
        _maxValues = new int[_numVertex];
        _instackList = new bool[_numVertex];
    }

    void addEdge(int id1, int id2) { //add edge to directed graph
        _adjLists[id1].push_back(id2);
    }

    void addVertex(int value, int id) {
        _lowList[id] = -1;
        _dtimeList[id] = -1;
        _valueList[id] = value;
        _instackList[id] = false;
        _maxValues[id] = value;
    }

    void printVertexList() {
        for (int i = 0; i < _numVertex; i++) {
            cout << _valueList[i] << endl;
        }
    }
};

void Tarjan_Visit(Graph *g, int id) {
    static int time = 0;
    g->_dtimeList[id] = g->_lowList[id] = ++time;
    g->_stack.push(id);
    g->_instackList[id] = true;
    for (list<int>::iterator i = g->_adjLists[id].begin(); i != g->_adjLists[id].end(); ++i) {
        int v = (*i);
        if (g->_dtimeList[v] == -1) { //v not visited yet
            Tarjan_Visit(g, v);
            g->_lowList[id] = min(g->_lowList[id], g->_lowList[v]);
        }

        else if(g->_instackList[v]) {
            g->_lowList[id] = min(g->_lowList[id], g->_dtimeList[v]); //according to the original tarjan

        }
        g->_maxValues[id] = max(g->_maxValues[id], g->_valueList[v]);
    }

    g->_valueList[id] = g->_maxValues[id];

    if (g->_dtimeList[id] == g->_lowList[id]) {
        while(g->_stack.top() != id) {
            g->_valueList[g->_stack.top()] = g->_maxValues[id];
            g->_instackList[g->_stack.top()] = false;
            g->_stack.pop();

        }
        g->_valueList[g->_stack.top()] = g->_maxValues[id];
        g->_stack.pop();
        g->_instackList[id] = false;
    }
}

void SCC_Tarjan(Graph *g) {
    for (int i = 0; i < g->_numVertex; i++) { 
        if (g->_dtimeList[i] == -1) {
            Tarjan_Visit(g, i);
        }
    }
}

Graph graph(0);

void processInput(int argc, char*argv[]) {
    int V;
    int E;

    string line;       

    getline(cin, line);

    sscanf(line.c_str(), "%d, %d", &V, &E);  
    graph.setV(V);
    int id = 0, id1 = 0, id2 = 0;  
    int i = 0;
    while (i < V) {
        getline(cin, line);
        graph.addVertex(atoi(line.c_str()), id);
        id++;
        i++;
    }
    i = 0;
    while(i < E) {
        getline(cin, line);
        sscanf(line.c_str(), "%d %d", &id1, &id2);
        graph.addEdge(id1 - 1, id2 - 1);
        i++;
    }
}

int main(int argc, char* argv[]) { 
    processInput(argc, argv);
    SCC_Tarjan(&graph);
    graph.printVertexList();
    return 0; 
}
