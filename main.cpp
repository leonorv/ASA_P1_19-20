#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
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
    list<vertex*> _stack;
    list<vertex*>::iterator _it;
    list<vertex*> _vertexList;
    list<vertex*> *_adjLists;

    public:

    Graph(int V) {
        _numVertex = V;
    }

    void setV(int numVertex) {
        _numVertex = numVertex;
        _adjLists = new list<vertex*>[numVertex];

    }

    void setE(int numEdges) {
        _numEdges = numEdges;
    }

    vertex* getVertex(int id) {
        list<vertex*>::iterator i = _vertexList.begin();
        advance(i, id - 1);
        return *i;
    }

    void addEdge(vertex *u, vertex *v) { //add edge to directed graph
        _adjLists[u->_id - 1].push_back(v);
    }

    void addVertex(int value, int id) {
        vertex *v = new vertex(value, id);
        _vertexList.push_back(v);
    }


    void Tarjan_Visit(vertex *u) {
        static int time = 0;
        u->_dtime = u->_low = ++time;
        _stack.push_back(u);
        u->_instack = true;
        for (list<vertex*>::iterator i = _adjLists[u->_id - 1].begin(); i != _adjLists[u->_id - 1].end(); ++i) {
            vertex *v = (*i);
            //_it = find(_stack.begin(), _stack.end(), v);  

            if (v->_dtime == -1) { //v not visited yet
                Tarjan_Visit(v);
                u->setLow(min(u->_low, v->_low));
                u->_value = max(u->_value, v->_value);
            }

            //else if (_it != _stack.end()) { //the element exists in stack
            else if(v->_instack) {
                u->setLow(min(u->_low, v->_dtime));
                u->_value = max(u->_value, v->_value);

            }
        }
        if (u->_dtime == u->_low) {
            while(_stack.back()->_id != u->_id) {
                _stack.pop_back();
            }
            _stack.pop_back();
        }

    }

    void SCC_Tarjan() {
        for (vertex *v : _vertexList) { 
            if (v->_dtime == -1) {
                Tarjan_Visit(v);
            }
        }

    }

    /*void printGraph() { //just to see the list
        for (int v = 1; v <= _numVertex; ++v) {
            cout << "\n Adjacency list of vertex "
                << v << "\n head "; 
            for (vertex *x : _adjLists[v - 1]) {
            cout << "-> " << x->_id; 
            }
            printf("\n"); 
        } 
    }*/

    void printVertexList() {
        for (vertex *v : _vertexList) {
            cout << v->_value << endl;
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
    int id = 1;  
    int i = 0;
    while (getline(cin, line)) {
        if (i < graph._numVertex) {
            graph.addVertex(atoi(line.c_str()), id);
            id++;
        }
        else {
            int id1, id2;
            sscanf(line.c_str(), "%d %d", &id1, &id2);
            graph.addEdge(graph.getVertex(id1), graph.getVertex(id2));
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