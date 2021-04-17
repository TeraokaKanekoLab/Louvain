#include "graph.hpp"

Graph::Graph()
{
    nb_nodes = 0;
    nb_links = 0;
    total_weight = 0;
}

Graph::Graph(string filepath)
{
    vector<vector<pair<int, float>>> all_links = read_file(filepath);
    renumber(all_links);

    // cumulative degree sequence
    unsigned long cumulative = 0;
    for (int i = 0; i < all_links.size(); ++i) {
        cumulative += all_links[i].size();
        degrees.push_back(cumulative);
    }

    // links
    for (int i = 0; i < all_links.size(); ++i) {
        for (int j = 0; j < all_links[i].size(); ++j) {
            links.push_back(all_links[i][j].first);
        }
    }
}

vector<vector<pair<int, float>>> Graph::read_file(string filepath)
{
    ifstream finput(filepath);
    assert(finput.good());

    // read from file
    vector<vector<pair<int, float>>> all_links;
    int u, v;
    while (finput >> u >> v) {
        if (all_links.size() <= max(u, v) + 1)
            all_links.resize(max(u, v) + 1);

        all_links[u].push_back(make_pair(v, 1));
        if (u != v)
            all_links[v].push_back(make_pair(u, 1));
        ++nb_links;
    }
    finput.close();

    return all_links;
}

void Graph::renumber(vector<vector<pair<int, float>>>& all_links)
{
    vector<bool> linked(all_links.size(), false);
    vector<int> renum(all_links.size(), -1);
    int nb = 0;

    for (unsigned int i = 0; i < all_links.size(); ++i) {
        for (unsigned int j = 0; j < all_links[i].size(); j++) {
            linked[i] = true;
            linked[all_links[i][j].first] = true;
        }
    }

    for (unsigned int i = 0; i < all_links.size(); ++i) {
        if (linked[i])
            renum[i] = nb++;
    }

    for (unsigned int i = 0; i < all_links.size(); ++i) {
        if (!linked[i])
            continue;
        for (unsigned int j = 0; j < all_links[i].size(); ++j) {
            all_links[i][j].first = renum[all_links[i][j].first];
        }
        all_links[renum[i]] = all_links[i];
    }
    all_links.resize(nb);
}

void Graph::display()
{
    for (unsigned int node = 0; node < nb_nodes; node++) {
        pair<vector<unsigned int>::iterator, vector<float>::iterator> p = neighbors(node);
        cout << node << ":";
        for (unsigned int i = 0; i < nb_neighbors(node); i++) {
            if (true) {
                if (weights.size() != 0)
                    cout << " (" << *(p.first + i) << " " << *(p.second + i) << ")";
                else
                    cout << " " << *(p.first + i);
            }
        }
        cout << endl;
    }
}

void Graph::display_reverse()
{
    for (unsigned int node = 0; node < nb_nodes; node++) {
        pair<vector<unsigned int>::iterator, vector<float>::iterator> p = neighbors(node);
        for (unsigned int i = 0; i < nb_neighbors(node); i++) {
            if (node > *(p.first + i)) {
                if (weights.size() != 0)
                    cout << *(p.first + i) << " " << node << " " << *(p.second + i) << endl;
                else
                    cout << *(p.first + i) << " " << node << endl;
            }
        }
    }
}

bool Graph::check_symmetry()
{
    int error = 0;
    for (unsigned int node = 0; node < nb_nodes; node++) {
        pair<vector<unsigned int>::iterator, vector<float>::iterator> p = neighbors(node);
        for (unsigned int i = 0; i < nb_neighbors(node); i++) {
            unsigned int neigh = *(p.first + i);
            float weight = *(p.second + i);

            pair<vector<unsigned int>::iterator, vector<float>::iterator> p_neigh = neighbors(neigh);
            for (unsigned int j = 0; j < nb_neighbors(neigh); j++) {
                unsigned int neigh_neigh = *(p_neigh.first + j);
                float neigh_weight = *(p_neigh.second + j);

                if (node == neigh_neigh && weight != neigh_weight) {
                    cout << node << " " << neigh << " " << weight << " " << neigh_weight << endl;
                    if (error++ == 10)
                        exit(0);
                }
            }
        }
    }
    return (error == 0);
}