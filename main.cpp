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
    list<int> *_adjLists;
    list<int> *_SCCLists;

    int  *_lowList;
    int  *_dtimeList;
    int  *_valueList;
    int *_maxValues;
    int *_indexList; // guarda os indexs do grafo original
    int *_sccIndexList; // guarda os indexs do grafo de sccs correspondente

    int *_headList; //saves scc head
    bool *_instackList;
    bool *_isSCChead; // vertex is head
    bool *_dfsVisited;

    bool *_addedToSCCGraph; //was added to scc graph


    list<int> *_connections;
    list<int> _SCCList;

    




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
        _sccIndexList = new int[_numVertex]; 
        _instackList = new bool[_numVertex];
        _isSCChead = new bool[_numVertex];
        _dfsVisited = new bool[_numVertex];

        _addedToSCCGraph = new bool[_numVertex];

        _headList = new int[_numVertex]; 


        //_connections = new list<int>[_numVertex];
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

        _indexList[id] = id; // keeps og indexs
        _sccIndexList[id] = id; // keeps og indexs

        _headList[id] = id;

        _addedToSCCGraph[id] = false;

    }

    void printVertexList() {
        for (int i = 0; i < _numVertex; i++) {
            cout << _valueList[i] << endl;
        }
    }

    void printGraph() { 
    for (int i = 0; i < _numVertex; ++i) { 
        cout << "\n Adjacency list of vertex "
             << _indexList[i]+1 << "\n head "; 
        for (auto x : _adjLists[i]) 
           cout << "-> " << _indexList[x] + 1; 
        printf("\n"); 
        } 
    } 
};

Graph graph(0);
Graph SCC_graph(0);

void printFinal(Graph *og, Graph *sccg) {
    for (int i = 0; i < og->_numVertex; i++) {
        //cout << _valueList[i] << endl;
        if (og->_isSCChead[i]) 
            cout << sccg->_valueList[og->_sccIndexList[i]] << endl;
        else 
            cout << sccg->_valueList[og->_sccIndexList[og->_headList[i]]] << endl;
        

    }
}


void create_SCC_graph(Graph *og, Graph *sccg) {
    list<int>::iterator j;
    int count = 0;
    sccg->setV(og->_numSCC);
    for (int i = 0; i < og->_numVertex; i++) {
        //printf("%d é head?%d      foi added? %d\n", i, og->_isSCChead[i], og->_addedToSCCGraph[i]);

        if (!og->_addedToSCCGraph[i] && og->_isSCChead[i]) {

            og->_addedToSCCGraph[i] = true;
            sccg->addVertex(og->_valueList[i], count);
            sccg->_indexList[count] = i;

            og->_sccIndexList[i] = count;
            count++; 
        }

            for (list<int>::iterator j = og->_adjLists[i].begin(); j != og->_adjLists[i].end(); ++j) {
                if (og->_isSCChead[(*j)] && og->_isSCChead[i]) {
                    if (!og->_addedToSCCGraph[(*j)]) {
                        og->_addedToSCCGraph[(*j)] = true;
                        sccg->addVertex(og->_valueList[(*j)], count);


                        og->_sccIndexList[(*j)] = count;
                        sccg->_indexList[count++] = (*j);
                    }
                    //printf("vai add %d com %d\n", i+1, (*j)+1);
                    if (i != *j) sccg->addEdge(og->_sccIndexList[i], og->_sccIndexList[(*j)]); //head a ligar head
                }

                else if (og->_isSCChead[i] && !og->_isSCChead[(*j)]) {
                    if (!og->_addedToSCCGraph[og->_headList[(*j)]]) {
                        og->_addedToSCCGraph[og->_headList[(*j)]] = true;
                        sccg->addVertex(og->_valueList[og->_headList[(*j)]], count);

                        og->_sccIndexList[og->_headList[(*j)]] = count;
                        sccg->_indexList[count++] = og->_headList[(*j)];
                    }

                    //printf("vai add %d com %d\n", i+1, og->_headList[(*j)]+1);
                    if (i != og->_headList[(*j)]) sccg->addEdge(og->_sccIndexList[i], og->_sccIndexList[og->_headList[(*j)]]); //head com head do scc
                }

                else if (!og->_isSCChead[i] && og->_isSCChead[(*j)]) {

                    if (!og->_addedToSCCGraph[og->_headList[i]]) {
                        og->_addedToSCCGraph[og->_headList[i]] = true;
                        sccg->addVertex(og->_valueList[og->_headList[i]], count);

                        og->_sccIndexList[og->_headList[i]] = count;
                        sccg->_indexList[count++] = og->_headList[i];
                    }
                    if (!og->_addedToSCCGraph[(*j)]) {
                        og->_addedToSCCGraph[(*j)] = true;
                        sccg->addVertex(og->_valueList[(*j)], count);

                        og->_sccIndexList[*j] = count;
                        sccg->_indexList[count++] = (*j);
                    }

                    if (og->_headList[i] != (*j)) sccg->addEdge(og->_sccIndexList[og->_headList[i]], og->_sccIndexList[(*j)]); //não head com head
                }

                else if (!og->_isSCChead[i] && !og->_isSCChead[(*j)]) {

                    if (!og->_addedToSCCGraph[og->_headList[i]]) {
                        og->_addedToSCCGraph[og->_headList[i]] = true;
                        sccg->addVertex(og->_valueList[og->_headList[i]], count);
                        og->_sccIndexList[og->_headList[i]] = count;
                        sccg->_indexList[count++] = og->_headList[i];
                    }
                    if (!og->_addedToSCCGraph[og->_headList[(*j)]]) {
                        og->_addedToSCCGraph[og->_headList[(*j)]] = true;
                        sccg->addVertex(og->_valueList[og->_headList[(*j)]], count);
                        og->_sccIndexList[og->_headList[*j]] = count;
                        sccg->_indexList[count++] = og->_headList[(*j)];
                    }
                    if (og->_headList[i] != og->_headList[(*j)]) sccg->addEdge(og->_sccIndexList[og->_headList[i]], og->_sccIndexList[og->_headList[(*j)]]); //não head com não head
                }
            }
    }
}



void DFS_Visit(Graph *g, int id) {
    g->_dfsVisited[id] = true;
    for (list<int>::iterator i = g->_adjLists[id].begin(); i != g->_adjLists[id].end(); ++i) {
        if (!g->_dfsVisited[(*i)]) DFS_Visit(g, (*i));
        g->_valueList[id] = max(g->_valueList[id], g->_valueList[(*i)]);
        
    }
}

void DFS(Graph *g) {
    for (int i = 0; i < g->_numVertex; i++) { 
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
        //printf("%d consegue ir a %d\n", id+1, (*i)+1);
        //g->_connections[id].push_back((*i));

        g->_maxValues[id] = max(g->_maxValues[id], g->_valueList[v]);
    }

    g->_valueList[id] = g->_maxValues[id];

    if (g->_dtimeList[id] == g->_lowList[id]) {
        while(g->_stack.top() != id) {
            g->_valueList[g->_stack.top()] = g->_maxValues[id];
            g->_headList[g->_stack.top()] = id; //saves head of scc
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
    //SCC_graph.printGraph();
    DFS(&SCC_graph);
    printFinal(&graph, &SCC_graph);
    //graph.printVertexList();
    return 0; 
}
