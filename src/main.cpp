#include "functions.hpp"

void test1(string filepath);

int main(int argc, char* argv[])
{
    test1(argv[1]);
    return 0;
}

void test1(string filepath)
{
    Graph graph = read_graph(filepath);
    CommunityGraph cg(graph);
    cg.move_community_into_another(1, 0);
    cg.move_community_into_another(2, 0);
    cg.move_community_into_another(3, 0);
    cg.move_community_into_another(5, 4);
    cg.move_community_into_another(6, 4);
    cg.move_community_into_another(8, 7);
    cg.move_community_into_another(9, 7);
    cg.print_communities();
    cg.print_weights();
    cg.print_edges();
    cg.print_neighbors();
    cg.print_vertices();
    cout << "modularity = " << cg.compute_modurality() << endl;
}
