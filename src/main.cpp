#include "functions.hpp"

void test1();
void test2();
void test3();

int main(int argc, char* argv[])
{

    return 0;
}

void test3()
{
    Graph graph = read_graph(argv[1]);
    CommunityGraph cg;
    cg.initialize_with_graph(graph);
    cg.print_communities();
    cg.print_weights();
    cg.print_edges();
    cg.print_neighbors();
    cg.print_vertices();
}
void test2()
{
    CommunityGraph cg;
    cg.add_community(0);
    cg.add_community(1);
    cg.add_community(2);
    cg.add_vertex_to_community(0, 0);
    cg.add_vertex_to_community(0, 1);
    cg.add_vertex_to_community(0, 2);
    cg.add_vertex_to_community(0, 3);
    cg.add_vertex_to_community(1, 4);
    cg.add_vertex_to_community(1, 5);
    cg.add_vertex_to_community(1, 6);
    cg.add_vertex_to_community(2, 7);
    cg.add_vertex_to_community(2, 8);
    cg.add_vertex_to_community(2, 9);
    cg.add_weight_to_edge(0, 0, 4);
    cg.add_weight_to_edge(0, 1, 1);
    cg.add_weight_to_edge(0, 2, 1);
    cg.add_weight_to_edge(1, 1, 3);
    cg.add_weight_to_edge(2, 2, 3);
    cg.print_communities();
    cg.print_weights();
    cg.print_edges();
    cg.print_neighbors();
    cg.print_vertices();
    cout << "modularity = " << cg.compute_modurality() << endl;
}

void test1()
{
    CommunityGraph cg;
    cg.add_community(0);
    cg.add_community(1);
    cg.add_community(2);
    cg.add_community(3);
    cg.add_community(4);
    cg.add_weight_to_edge(0, 1, 3);
    cg.add_weight_to_edge(1, 1, 5);
    cg.add_weight_to_edge(0, 3, 7);
    cg.add_weight_to_edge(0, 4, 8);
    cg.add_weight_to_edge(1, 2, 1);
    cg.add_weight_to_edge(1, 3, 4);
    cg.add_weight_to_edge(2, 3, 2);
    cg.add_weight_to_edge(3, 4, 3);
    cg.add_vertex_to_community(0, 11);
    cg.add_vertex_to_community(0, 12);
    cg.add_vertex_to_community(0, 13);
    cg.add_vertex_to_community(1, 14);
    cg.add_vertex_to_community(1, 15);
    cg.add_vertex_to_community(2, 16);
    cg.add_vertex_to_community(2, 17);
    cg.add_vertex_to_community(3, 18);
    cg.add_vertex_to_community(3, 19);
    cg.add_vertex_to_community(3, 20);
    cg.add_vertex_to_community(4, 21);
    cg.add_vertex_to_community(4, 22);
    cg.remove_community(1);
    cg.print_communities();
    cg.print_weights();
    cg.print_edges();
    cg.print_neighbors();
    cg.print_vertices();
}