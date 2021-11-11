#include "graph.cpp"

class MergedGraph : public Graph {
private:
    Graph g;
    Graph h;

public:
    MergedGraph(Graph g1, Graph g2)
    {
        g = g1;
        h = g2;
        ofstream output("graph/tmp.gr");
        for (int v = 0; v < g1.num_nodes; ++v) {
            int first_neighbor = g1.neighbors(v).first;
            int degree = g1.num_neighbors(v);
            for (int i = first_neighbor; i < first_neighbor + degree; ++i) {
                int nbr = g1.links[i];
                if (i < nbr)
                    output << i << " " << nbr;
            }
        }
        // for (int v = 0; v < g2.num_nodes; ++v)
        //     for (int n : g2.get_neighbors(v))
        //         if (v < n)
        //             cout << v + g.num_nodes << " " << n + g.num_nodes << endl;
    }
};