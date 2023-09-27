#include <iostream>
#include <unordered_set>
#include <algorithm>

#include "Graph.h"


struct vertex_level
{
    int vertex;
    int level;
};


GraphAdjList::GraphAdjList(unsigned int Vertixes, ConnectionType type)
    : Graph(Vertixes, type)
{
    m_list.resize(Vertixes);
}

void GraphAdjList::addEdge(int v, int w)
{
    //Check if w is not already in the list of v
    m_list[v].push_back(w);

    if (m_connectionType == ConnectionType::undirected)
    {
        m_list[w].push_back(v);
    }
}


void GraphAdjList::BFS(int vertex)
{
    // Mark all the vertices as not visited
    std::unordered_set<int> visited;
    BFS(vertex, visited);
}

void GraphAdjList::BFS(int vertex, std::unordered_set<int>& visited) const
{
    std::list<int> queue;

    // Mark the current node as visited and enqueue it
    visited.insert(vertex);
    queue.push_back(vertex);

    while (!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        int currentVertex = queue.front();

        queue.pop_front();

        // Get all adjacent vertices
        for (int adjacent : m_list[currentVertex])
        {
            if (!visited.contains(adjacent))
            {
                visited.insert(adjacent);
                queue.push_back(adjacent);
            }
        }
    }
}

int GraphAdjList::numberOfNodesInLevel(int vertex, int level) const
{
    int vertex_count{ 0 };

    std::unordered_set<int> visited;
    std::list<vertex_level> queue;

    visited.insert(vertex);
    queue.push_back({ vertex, 0 });

    while (!queue.empty())
    {
        auto [currentVertex, currentLevel] = queue.front();
        queue.pop_front();

        if (currentLevel > level)
        {
            break;
        }

        if (currentLevel == level)
        {
            ++vertex_count;
        }

        ++currentLevel;
        for (int adjacent : m_list[currentVertex])
        {
            if (!visited.contains(adjacent))
            {
                visited.insert(adjacent);
                queue.push_back({ adjacent, currentLevel });
            }
        }
    }

    return vertex_count;
}

bool GraphAdjList::isStronglyConnected() const
{
    if (m_connectionType == ConnectionType::undirected)
    {
        return false;
    }

    std::unordered_set<int> visited_vertex;
    BFS(0, visited_vertex);

    if (visited_vertex.size() != m_vertixes)
    {
        return false;
    }

    GraphAdjList reversed_graph{ m_vertixes, m_connectionType };

    for (unsigned int vertex = 0; vertex < m_vertixes; ++vertex)
    {
        for (const int adjacent : m_list[vertex])
        {
            reversed_graph.addEdge(adjacent, vertex);
        }
    }

    std::unordered_set<int> visited_vertex_reversed;
    reversed_graph.BFS(0, visited_vertex_reversed);

    if (visited_vertex.size() != visited_vertex_reversed.size())
    {
        return false;
    }

    return !std::ranges::any_of(visited_vertex, [&visited_vertex_reversed](const int vertex) {
        return !visited_vertex_reversed.contains(vertex);
    });
}

GraphAdjMatrix::GraphAdjMatrix(const int vertices, const ConnectionType type) : Graph(vertices, type)
{
    m_matrix.resize(vertices, std::vector<unsigned>(m_vertixes, 0));
}

void GraphAdjMatrix::addEdge(int v, int w)
{
    m_matrix[v][w] = 1;

    if (m_connectionType == ConnectionType::undirected)
    {
        m_matrix[w][v] = 1;
    }
}

void GraphAdjMatrix::DFS(int vertex)
{
    std::unordered_set<unsigned> visited;
    DFS(vertex, visited);
}

void GraphAdjMatrix::DFS(unsigned vertex, std::unordered_set<unsigned>& visited) const
{
    visited.insert(vertex);

    std::cout << vertex << " ";

    for (unsigned i = 0; i < m_vertixes; ++i)
    {
        if (m_matrix[vertex][i] && !visited.contains(i))
        {
            DFS(i, visited);
        }
    }
}
