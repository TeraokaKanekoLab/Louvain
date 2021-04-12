#include "header.hpp"

class Graph {
private:
    typedef pair<int, int> edge;
    unordered_map<int, unordered_set<int>> neighbors;
    int array_size;
    int num_edges;

    void add_vertex(int v)
    {
        if (has_vertex(v))
            return;
        neighbors[v] = unordered_set<int>();
    }

public:
    // getter
    bool has_vertex(int v)
    {
        return neighbors.find(v) != neighbors.end();
    }

    // We assume v is a valid node
    // Otherwise an exception will be triggered
    int get_degree(int v)
    {
        return neighbors[v].size();
    }

    // We assume v is a valid node
    // Otherwise an exception will be triggered
    unordered_set<int> get_neighbors(int v)
    {
        return neighbors[v];
    }

    void print_graph()
    {
        for (auto [u, nbrs] : neighbors) {
            cout << u << ":";
            for (auto v : nbrs) {
                cout << " " << v;
            }
            cout << endl;
        }
    }

    // setter
    void add_edge(int u, int v)
    {
        add_vertex(u);
        add_vertex(v);
        neighbors[u].insert(v);
        neighbors[v].insert(u);
    }
};