#include "graph.hpp"

// A hash function used to hash a pair of any kind
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1> {}(p.first);
        auto hash2 = hash<T2> {}(p.second);
        return hash1 ^ hash2;
    }
};

class CommunityGraph {
private:
    // typedef
    typedef pair<int, int> edge;

    // community
    unordered_map<int, unordered_set<int>> nodes_in_community;
    unordered_map<int, unordered_set<int>> neighbors;
    unordered_map<int, int> inside_weights;
    unordered_map<int, int> attached_weights;

    // edges
    unordered_map<edge, int, hash_pair> edge_weights;

    // vertices
    unordered_map<int, int> community_of_vertices;

    int sum_weights;

public:
    CommunityGraph(Graph graph)
    {
        sum_weights = 0;
        vector<int> vertices = graph.get_vertices();
        for (auto v : vertices) {
            add_community(v);
            add_vertex_to_community(v, v);
        }
        for (auto v : vertices) {
            for (auto nbr : graph.get_neighbors(v)) {
                if (v < nbr)
                    add_weight_to_edge(v, nbr, 1);
            }
        }

        sum_weights = graph.get_num_edges();
    }

    // ---------------------------------------------------
    // getters
    // ---------------------------------------------------
    bool
    has_community(int c)
    {
        return nodes_in_community.find(c) != nodes_in_community.end();
    }

    bool has_vertex(int v)
    {
        return community_of_vertices.find(v) != community_of_vertices.end();
    }

    bool has_edge(edge e)
    {
        return edge_weights.find(e) != edge_weights.end();
    }

    bool has_vertex_in_community(int c, int v)
    {
        if (!has_vertex(v)) {
            cout << "vertex " << c << " not exists" << endl;
            return false;
        }

        return community_of_vertices[v] == c;
    }

    int get_edge_weight(int c1, int c2)
    {
        if (!has_edge(edge(c1, c2)))
            return -1;
        return edge_weights[edge(c1, c2)];
    }

    vector<int> get_neighbors(int c)
    {
        vector<int> nbrs;
        for (auto nbr : neighbors[c])
            nbrs.push_back(nbr);
        return nbrs;
    }

    vector<int> get_communities()
    {
        vector<int> communities;
        for (auto [c, nodes] : nodes_in_community) {
            communities.push_back(c);
        }
        return communities;
    }

    // ---------------------------------------------------
    // setters
    // ---------------------------------------------------

    void add_community(int c)
    {
        if (has_community(c)) {
            cout << "community " << c << " already exists" << endl;
            return;
        }

        nodes_in_community[c] = unordered_set<int>();
        neighbors[c] = unordered_set<int>();
        inside_weights[c] = 0;
        attached_weights[c] = 0;
    }

    void add_vertex_to_community(int c, int v)
    {
        if (!has_community(c)) {
            cout << "community not exists" << endl;
            return;
        }

        nodes_in_community[c].insert(v);
        community_of_vertices[v] = c;
    }

    void remove_vertex_from_community(int c, int v)
    {
        if (!has_community(c)) {
            cout << "community not exists" << endl;
            return;
        }
        if (!has_vertex(v)) {
            cout << "vertex not exists" << endl;
            return;
        }
        if (!has_vertex(v)) {
            cout << "vertex not exists" << endl;
            return;
        }

        nodes_in_community[c].erase(v);
        community_of_vertices.erase(v);
    }

    void add_weight_to_edge(int c1, int c2, int weight)
    {
        if (!has_community(c1) || !has_community(c2)) {
            cout << "community not exists" << endl;
            return;
        }

        edge e1 = edge(c1, c2);
        edge e2 = edge(c2, c1);

        if (!has_edge(e1) || !has_edge(e2)) {
            edge_weights[e1] = 0;
            edge_weights[e2] = 0;
            neighbors[c1].insert(c2);
            neighbors[c2].insert(c1);
        }

        edge_weights[e1] += weight;
        edge_weights[e2] += weight;
        attached_weights[c1] += weight;
        attached_weights[c2] += weight;

        if (c1 == c2) {
            edge_weights[e1] -= weight;
            attached_weights[c1] -= weight;
            inside_weights[c1] += weight;
        }
    }

    void remove_edge(int c1, int c2)
    {
        if (!has_community(c1) || !has_community(c2)) {
            cout << "community not exists" << endl;
            return;
        }

        edge e1 = edge(c1, c2);
        edge e2 = edge(c2, c1);

        if (!has_edge(e1) || !has_edge(e2)) {
            cout << "edge not exists" << endl;
            return;
        }
        attached_weights[c1] -= edge_weights[e1];
        attached_weights[c2] -= edge_weights[e1];
        if (c1 == c2) {
            inside_weights.erase(c1);
            edge_weights.erase(e1);
            neighbors[c1].erase(c2);
            return;
        }
        edge_weights.erase(e1);
        edge_weights.erase(e2);
        neighbors[c1].erase(c2);
        neighbors[c2].erase(c1);
    }

    void remove_community(int c)
    {
        if (!has_community(c)) {
            cout << "community not exists" << endl;
            return;
        }
        for (auto neighbor : get_neighbors(c))
            remove_edge(c, neighbor);
        if (has_edge(edge(c, c)))
            remove_edge(c, c);

        neighbors.erase(c);
        nodes_in_community.erase(c);
        inside_weights.erase(c);
        attached_weights.erase(c);
    }

    void move_community_into_another(int s, int t)
    {
        if (!has_community(s) || !has_community(t)) {
            cout << "community not exists" << endl;
            return;
        }

        if (s == t) {
            cout << "communities must not be same" << endl;
            return;
        }

        // merge s to t
        for (auto v : nodes_in_community[s]) {
            nodes_in_community[t].insert(v);
            community_of_vertices[v] = t;
        }
        for (auto nbr : neighbors[s]) {
            if (nbr == s)
                continue;
            if (nbr == t) {
                add_weight_to_edge(t, t, edge_weights[edge(s, t)]);
            }
            add_weight_to_edge(t, nbr, edge_weights[edge(s, nbr)]);
        }
        add_weight_to_edge(t, t, edge_weights[edge(s, s)]);

        remove_community(s);
    }

    // ---------------------------------------------------
    // computation
    // ---------------------------------------------------

    double compute_modurality()
    {
        double modularity = 0;
        for (auto [c, inside_weight] : inside_weights) {
            modularity += (double)inside_weight / (2 * sum_weights);
            int attached_weight = attached_weights[c];
            modularity -= (double)attached_weight * attached_weight / (4 * sum_weights * sum_weights);
        }
        return modularity;
    }

    // ---------------------------------------------------
    // Louvain
    // ---------------------------------------------------

    double louvain()
    {

        double prev = compute_modurality();
        // for all community
        while (true) {
            // randomize the order of vertices
            vector<int> communities = get_communities();
            random_device rd;
            mt19937 g(rd());
            shuffle(communities.begin(), communities.end(), g);

            double res = 0;
            for (int i = 0; i < communities.size(); ++i) {
                int best_community = i;
                double best_increase = 0;
                for (int j = 0; j < communities.size(); ++j) {
                    if (i == j)
                        continue;
                    if (!has_community(communities[j]))
                        continue;
                    double sigma_in = inside_weights[j];
                    double sigma_tot = attached_weights[j];
                    double ki = attached_weights[i];
                    double ki_in = edge_weights[edge(i, j)];
                    double delta_q = (sigma_in + 2 * ki_in) / (2 * sum_weights) - (sigma_tot + ki) * (sigma_tot + ki) / (4 * sum_weights * sum_weights);
                    delta_q -= sigma_in / (2 * sum_weights) - sigma_tot * sigma_tot / (4 * sum_weights * sum_weights) - ki * ki / (4 * sum_weights * sum_weights);
                    // double delta_q2 = ki_in / sum_weights - sigma_tot * ki / (2 * sum_weights * sum_weights);
                    // cout << "delta_q = " << delta_q << ", delta_q2 = " << delta_q2 << endl;
                    if (delta_q > best_increase) {
                        best_increase = delta_q;
                        best_community = j;
                    }
                }
                if (best_increase <= 0)
                    continue;
                move_community_into_another(i, best_community);
                res = compute_modurality();
            }
            if (prev == res)
                break;
            prev = res;
        }
        return prev;
    }

    // ---------------------------------------------------
    // debug
    // ---------------------------------------------------

    void print_communities()
    {
        cout << "communities" << endl;
        for (auto [community, vertices] : nodes_in_community) {
            cout << community << ":";
            for (auto vertex : vertices) {
                cout << " " << vertex;
            }
            cout << endl;
        }
    }

    void print_weights()
    {
        cout << "weight" << endl;
        for (auto [community, weight] : inside_weights) {
            cout << community << "; inside: " << weight << ", attached: " << attached_weights[community] << endl;
        }
    }

    void print_edges()
    {
        cout << "edges" << endl;
        for (auto [edge, weight] : edge_weights) {
            cout << "(" << edge.first << ", " << edge.second << "): " << weight << endl;
        }
    }

    void print_neighbors()
    {
        cout << "neighbors" << endl;
        for (auto [v, nbrs] : neighbors) {
            cout << v << ":";
            for (auto nbr : nbrs) {
                cout << " " << nbr;
            }
            cout << endl;
        }
    }

    void print_vertices()
    {
        cout << "vertices" << endl;
        for (auto [v, c] : community_of_vertices)
            cout << v << ": " << c << endl;
    }
};