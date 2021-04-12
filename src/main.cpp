#include "functions.hpp"

int main(int argc, char* argv[])
{
    Graph graph = read_graph(argv[1]);
    vector<vector<int>> communities {
        { 0, 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };
    cout << "modularity: " << graph.compute_modularity(communities) << endl;

    return 0;
}