#pragma once

#include <list>
#include <vector>
#include <unordered_set>

class Graph
{
public:
    enum class ConnectionType
    {
        undirected,
        directed
    };

    Graph(unsigned int vertixes, ConnectionType type = ConnectionType::undirected)
        : m_vertixes(vertixes), m_connectionType(type) {}

    virtual void addEdge(int v, int w) = 0;

    virtual void BFS(int vertex = 0) {};
    virtual void DFS(int vertex = 0) {};

protected:
    unsigned int m_vertixes;
    ConnectionType m_connectionType;
};

// using adjacency list
class GraphAdjList : public Graph
{
public:
    GraphAdjList(unsigned int vertixes, ConnectionType type = ConnectionType::undirected);
    void addEdge(int v, int w) override;

    void BFS(int vertex = 0) override;

    void BFS(int vertex, std::unordered_set<int>& visited) const;

    int numberOfNodesInLevel(int vertex, int level) const; //#TODO IMPLEMLENT

    bool isStronglyConnected() const; //#TODO IMPLEMENT

private:
    std::vector<std::list<int> > m_list;
};

//#TODO HW
class GraphAdjMatrix : public Graph
{
public:
    GraphAdjMatrix(const int vertices, const ConnectionType type = ConnectionType::undirected); //#TODO, IMPLEMENT if needed

    void addEdge(int v, int w) override; //#TODO IMPLEMENT

    void DFS(int vertex = 0) override; //#TODO IMPLEMENT

    void DFS(unsigned vertex, std::unordered_set<unsigned>& visited) const;
private:
    std::vector<std::vector<unsigned>> m_matrix;
};
