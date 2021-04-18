#include "header.hpp"

class Graph {
public:
    unsigned int nb_nodes;
    unsigned long nb_links;
    double total_weight;

    vector<unsigned long> degrees;
    vector<unsigned int> links;
    vector<float> weights;

    Graph();
    Graph(string filepath, int type);
    vector<vector<pair<int, float>>> read_file(string filepath);
    void renumber(vector<vector<pair<int, float>>>& all_links);

    void display();

    inline unsigned int nb_neighbors(unsigned int node);
    inline int nb_selfloops(unsigned int node);
    inline double weighted_degree(unsigned int node);

    // return pointers to the first neighbor and first weight of the node
    inline pair<vector<unsigned int>::iterator, vector<float>::iterator> neighbors(unsigned int node);

    void print_links()
    {
        cout << "[" << links[0];
        for (int i = 1; i < links.size(); ++i) {
            cout << ", " << links[i];
        }
        cout << "]" << endl;
    }

    void print_degrees()
    {
        cout << "[" << degrees[0];
        for (int i = 1; i < degrees.size(); ++i) {
            cout << ", " << degrees[i];
        }
        cout << "]" << endl;
    }
};

inline unsigned int Graph::nb_neighbors(unsigned int node)
{
    assert(node >= 0 && node < nb_nodes);

    if (node == 0)
        return degrees[0];
    else
        return degrees[node] - degrees[node - 1]; // cumulative sum?
}

// This method doesn't seem efficient
inline int Graph::nb_selfloops(unsigned int node)
{
    assert(node >= 0 && node < nb_nodes);
    pair<vector<unsigned int>::iterator, vector<float>::iterator> p = neighbors(node);
    for (unsigned int i = 0; i < nb_neighbors(node); ++i) {
        if (*(p.first + i) != node)
            continue;
        if (weights.size() == 0)
            return 1;
        return *(p.second + i);
    }

    return 0;
}

inline double Graph::weighted_degree(unsigned int node)
{
    assert(node >= 0 && node < nb_nodes);

    pair<vector<unsigned int>::iterator, vector<float>::iterator> p = neighbors(node);
    if (p.second == weights.end())
        return nb_neighbors(node);
    else {
        int res = 0;
        for (int i = 0; i < nb_neighbors(node); i++)
            res += *(p.second + i);
        return res;
    }
}

inline pair<vector<unsigned int>::iterator, vector<float>::iterator> Graph::neighbors(unsigned int node)
{
    assert(node >= 0 && node < nb_nodes);

    if (node == 0)
        return make_pair(links.begin(), weights.begin());
    if (weights.size() == 0)
        return make_pair(links.begin() + degrees[node - 1], weights.begin());
    return make_pair(links.begin() + degrees[node - 1], weights.begin() + degrees[node - 1]);
}