#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <vector>
using namespace std;

struct vertex {
    int _id;
    int _value;
    int _low;
    int _dtime;
    int _visited;
    bool _instack;
    vertex *_pi;

    public:
    vertex(int value, int id) {
        _value = value;
         _id = id;
        _visited = 0;
        _dtime = -1;
        _low = -1;
        _instack = false;

    }

    bool operator==(const vertex *v) const {
     return _id == v->_id;
  }

  void setID(int id) {
      _id = id;
  }

  void setLow(int low) {
      _low = low;
  }

  void setDTime(int dtime) {
      _dtime = dtime;
  }

};

struct Graph {
    int _numVertex;
    int _numEdges;
    stack<int> _stack; 
    list<int> *_adjLists;
    list<int> *_sccLists;

    int  *_lowList;
    int  *_dtimeList;
    int  *_valueList;
    bool *_instackList;



    public:

    Graph(int V) {
        _numVertex = V;
    }

    void setV(int numVertex) {
        _numVertex = numVertex;
        _adjLists = new list<int>[numVertex];
        _lowList = new int[_numVertex];
        _dtimeList = new int[_numVertex];
        _valueList = new int[_numVertex];
        _instackList = new bool[_numVertex];
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
    }


    void Tarjan_Visit(int id) {
        static int time = 0;
        _dtimeList[id] = _lowList[id] = ++time;
        _stack.push(id);
        _instackList[id] = true;
        vector<int> sccVec;
        int maxValue = _valueList[id];
        for (list<int>::iterator i = _adjLists[id].begin(); i != _adjLists[id].end(); ++i) {
            int v = (*i);
            if (_dtimeList[v] == -1) { //v not visited yet
                Tarjan_Visit(v);
                _lowList[id] = min(_lowList[id], _lowList[v]);
            }

            else if(_instackList[v]) {
                _lowList[id] = min(_lowList[id], _lowList[v]);

            }
            _valueList[id] = max(_valueList[id], _valueList[v]);

        }
        if (_dtimeList[id] == _lowList[id]) {
            while(_stack.top() != id) {
                _instackList[_stack.top()] = false;
                sccVec.push_back(_stack.top());
                maxValue = max(maxValue, _valueList[_stack.top()]);
                _stack.pop();

            }
            _stack.pop();
            _instackList[id] = false;

            for (vector<int>::iterator i = sccVec.begin(); i != sccVec.end(); ++i) {
                _valueList[(*i)] = maxValue;
            }
            

        }


        

    }

    void SCC_Tarjan() {
        for (int i = 0; i < _numVertex; i++) { 
            if (_dtimeList[i] == -1) {
                Tarjan_Visit(i);
            }
        }

    }

    void printVertexList() {
        for (int i = 0; i < _numVertex; i++) {
            cout << _valueList[i] << endl;
        }
    }
};

Graph graph(0);

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
    graph.SCC_Tarjan();
    graph.printVertexList();
    return 0; 
}