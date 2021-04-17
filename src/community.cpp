#include "community.hpp"

Community::Community(string filename, int nbp, double minm)
{
    g = Graph(filename);
    size = g.nb_nodes;

    n2c.resize(size);
    in.resize(size);
    tot.resize(size);

    for (int i = 0; i < size; ++i) {
        n2c[i] = i;
        in[i] = g.nb_selfloops(i);
        tot[i] = g.weighted_degree(i);
    }

    nb_pass = nbp;
    min_modularity = minm;
}

Community::Community(Graph gc, int nbp, double minm)
{
    g = gc;
    size = g.nb_nodes;

    n2c.resize(size);
    in.resize(size);
    tot.resize(size);

    for (int i = 0; i < size; ++i) {
        n2c[i] = i;
        in[i] = g.nb_selfloops(i);
        tot[i] = g.weighted_degree(i);
    }

    nb_pass = nbp;
    min_modularity = minm;
}

void Community::display()
{
    cerr << endl
         << "<";
    for (int i = 0; i < size; i++)
        cerr << " " << i << "/" << n2c[i] << "/" << in[i] << "/" << tot[i];
    cerr << ">" << endl;
}

double Community::modularity()
{
    double q = 0.;
    double m2 = (double)g.total_weight;

    for (int i = 0; i < size; ++i) {
        if (tot[i] > 0)
            q += (double)in[i] / m2 - (double(tot[i] / m2) * ((double)tot[i] / m2));
    }
}

map<int, int> Community::neigh_comm(int node)
{
    map<int, int> res;
    pair<vector<unsigned int>::iterator, vector<float>::iterator> p = g.neighbors(node);

    int deg = g.nb_neighbors(node);

    res.insert(make_pair(n2c[node], 0));

    for (int i = 0; i < deg; ++i) {
        int neigh = *(p.first + i);
        int neigh_comm = n2c[neigh];
        int neigh_weight = 1;

        if (neigh == node)
            continue;
        map<int, int>::iterator it = res.find(neigh_comm);
        if (it != res.end())
            it->second += neigh_weight;
        else
            res.insert(make_pair(neigh_comm, neigh_weight));
    }

    return res;
}