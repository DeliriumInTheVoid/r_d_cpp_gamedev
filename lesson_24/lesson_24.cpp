#include <iostream>

#include "console.hpp"
#include "Graph.h"


void print_menu();

void fill_undirected_graph(Graph& graph);
void fill_directed_graph(Graph& graph);

void graph_adj_list_vertices_on_level();
void graph_adj_list_is_strongly_connected();
void graph_adj_matrix_dfs();

int main()
{
    std::cout << "Home Work 24\n";
    std::cout << "-----------\n\n";

    int user_choice{ 0 };
    while (user_choice != 4)
    {
        print_menu();
        std::cin >> user_choice;
        utl::clear_console();
        switch (user_choice)
        {
        case 1:
            graph_adj_list_vertices_on_level();
            break;
        case 2:
            graph_adj_list_is_strongly_connected();
            break;
        case 3:
            graph_adj_matrix_dfs();
            break;
        case 4:
            std::cout << "Exit\n";
            break;
        default:
            std::cout << "Wrong choice!";
            user_choice = 0;
            break;
        }
        utl::wait_to_continue();
    }

    return 0;
}


void print_menu()
{
    std::cout << "1. graph adjacency list vertex on level\n";
    std::cout << "2. graph adjacency list is strongly connected\n";
    std::cout << "3. graph adjacency matrix dfs\n";
    std::cout << "4. Exit\n\n";

    std::cout << "Enter number:";
}

void fill_undirected_graph(Graph& graph)
{
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(0, 3);

    graph.addEdge(1, 4);
    graph.addEdge(2, 5);
    graph.addEdge(2, 6);
    graph.addEdge(3, 7);

    graph.addEdge(4, 8);
    graph.addEdge(5, 9);
    graph.addEdge(6, 9);
    graph.addEdge(1, 9);
    graph.addEdge(1, 3);
}

void fill_directed_graph(Graph& graph)
{
    graph.addEdge(0, 1);
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    graph.addEdge(3, 0);
    graph.addEdge(2, 4);
    graph.addEdge(4, 5);
    graph.addEdge(5, 6);
    graph.addEdge(6, 7);
    graph.addEdge(7, 1);
}

void graph_adj_list_vertices_on_level()
{
    GraphAdjList graph(10);

    fill_undirected_graph(graph);

    constexpr int vertex{ 0 };
    constexpr int level{ 2 };

    std::cout << "Vertices on level " << level
        << " are " << graph.numberOfNodesInLevel(vertex, level)
        << ", starting from: " << vertex << " vertex";
}

void graph_adj_list_is_strongly_connected()
{
    GraphAdjList graph(8, Graph::ConnectionType::directed);

    fill_directed_graph(graph);

    std::cout << "graph is strongly connected: " << std::boolalpha << graph.isStronglyConnected();
}

void graph_adj_matrix_dfs()
{
    {
        std::cout << "DFS for directed graph\n";
        GraphAdjMatrix graph(8, Graph::ConnectionType::directed);
        fill_directed_graph(graph);
        graph.DFS();
    }
    std::cout << std::endl;
    {
        std::cout << "DFS for undirected graph\n";
        GraphAdjMatrix graph(10, Graph::ConnectionType::undirected);
        fill_undirected_graph(graph);
        graph.DFS();
    }
}
