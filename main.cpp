#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <vector>
using namespace std;

struct Graph {
    int _numVertex;
    int _numEdges;
    int _numSCC;
    stack<int> _stack; 
    list<int> _SCCList;
    list<int> *_adjLists;
    list<int> *_SCCLists;

    int  *_lowList;
    int  *_dtimeList;
    int  *_valueList;
    int *_maxValues;
    int *_indexList;
    bool *_instackList;
    bool *_isSCChead;
    bool *_dfsVisited;


    public:

    Graph(int V) {
        _numVertex = V;
    }

    void setV(int numVertex) {
        _numVertex = numVertex;
        _numSCC = 0;
        _adjLists = new list<int>[_numVertex];
        _SCCLists = new list<int>[_numVertex];
        _lowList = new int[_numVertex];
        _dtimeList = new int[_numVertex];
        _valueList = new int[_numVertex];
        _maxValues = new int[_numVertex];
        _indexList = new int[_numVertex];
        _instackList = new bool[_numVertex];
        _isSCChead = new bool[_numVertex];
        _dfsVisited = new bool[_numVertex];
    }

    void setE(int numEdges) {
        _numEdges = numEdges;
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
        _isSCChead[id] = false;
        _dfsVisited[id] = false;
        _indexList[id] = id;
    }

    void printVertexList() {
        for (int i = 0; i < _numVertex; i++) {
            cout << _valueList[i] << endl;
        }
    }
};

Graph graph(0);
Graph SCC_graph(0);

void create_SCC_graph(Graph *og, Graph *sccg) {
    printf("há %d sccs\n", og->_numSCC);
    for (int i : og->_SCCList) {
        for (list<int>::iterator i2 = og->_SCCLists[i].begin(); i2 != og->_SCCLists[i].end(); ++i2) {
            if (og->_isSCChead[(*i2)]) {
                printf("%d liga a %d\n", og->_indexList[i] + 1, og->_indexList[(*i2)] + 1);
                sccg->addEdge(i, (*i2));
            }
            else printf("%d não é head e estamos a analisar %d\n", (*i2) + 1, i + 1);
        }
    }
}

void updateGraph(Graph *og, Graph *sccg) {
    for (int i = 0; i < og->_numVertex; i++) {
        //if (sccg->_valueList[i] != NULL)
            og->_valueList[i] = sccg->_valueList[i];
    }
}

void DFS_Visit(Graph *g, int id) {
    g->_dfsVisited[id] = true;
    for (list<int>::iterator i = g->_adjLists[id].begin(); i != g->_adjLists[id].end(); ++i) {
        if (!g->_dfsVisited[(*i)]) DFS_Visit(g, (*i));
        //g->_valueList[id] = max(g->_valueList[id], g->_valueList[(*i)]);
        printf("oi\n");
        g->_valueList[id] = 22;
    }

}

void DFS(Graph *g) {
    for (int i = 0; i < g->_numVertex; i++) { 
        printf("vamos fazer dfs visit %d\n", i + 1);
        DFS_Visit(g, i);
    }
}

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
            g->_SCCLists[id].push_back(g->_stack.top()); //meter na lista de sccs da head
            g->_instackList[g->_stack.top()] = false;
            g->_stack.pop();

        }
        g->_valueList[g->_stack.top()] = g->_maxValues[id];
        g->_numSCC++;
        g->_SCCList.push_back(id);
        g->_isSCChead[g->_stack.top()] = true; 
        g->_stack.pop();
        g->_instackList[id] = false;
    }
}

void SCC_Tarjan(Graph *g) {
    SCC_graph.setV(g->_numVertex);
    for (int i = 0; i < g->_numVertex; i++) { 
        if (g->_dtimeList[i] == -1) {
            Tarjan_Visit(g, i);
        }
    }
}

void processInput(int argc, char*argv[]) {
    int V;
    int E;

    string line;       

    getline(cin, line);

    sscanf(line.c_str(), "%d, %d", &V, &E);  
    graph.setV(V);
    int id = 0;  
    int i = 0;
    while (getline(cin, line)) {
        if (i < graph._numVertex) {
            graph.addVertex(atoi(line.c_str()), id);
            id++;
        }
        else {
            int id1, id2;
            sscanf(line.c_str(), "%d %d", &id1, &id2);
            graph.addEdge(id1 - 1, id2 - 1);
        }
        i++;
  }
}

int main(int argc, char* argv[]) { 
    processInput(argc, argv);
    SCC_Tarjan(&graph);
    create_SCC_graph(&graph, &SCC_graph);
    DFS(&SCC_graph);
    graph.printVertexList();
    return 0; 
}
