#include "functions.hpp"

int main(int argc, char* argv[])
{
    Graph graph = read_graph(argv[1]);
    graph.print_graph();
    return 0;
}