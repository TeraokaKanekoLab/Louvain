#include "community.cpp"

#define PRECISION 0.000001
#define DISPLAY_LEVEL -2

void display_time(const char *str)
{
    time_t rawtime;
    time(&rawtime);
    cerr << str << " : " << ctime(&rawtime);
}

void update_original_node_community(unordered_map<int, int> &original_id_to_community, vector<int> &community_of, unordered_map<int, int> &renum)
{
    for (auto [node, c] : original_id_to_community)
    {
        int community = community_of[c];
        int new_community = renum[community];
        original_id_to_community[node] = new_community;
    }
}

void print_partition(std::unordered_map<int, std::vector<int>> x)
{

    for (auto [nd, c] : x)
    {
        cout << nd << ":";
        for (auto ndc : c)
        {
            cout << " " << ndc;
        }
        cout << endl;
    }
}

void print_partitions(vector<std::unordered_map<int, std::vector<int>>> y)
{

    for (auto x : y)
    {
        for (auto [nd, c] : x)
        {
            cout << nd << ":";
            for (auto ndc : c)
            {
                cout << " " << ndc;
            }
            cout << endl;
        }
        cout << endl;
    }
}

unordered_map<int, vector<int>> get_partition(unordered_map<int, int> original_id_to_community, std::unordered_map<int, std::vector<int>> &clusters)
{
    unordered_map<int, vector<int>> c2nodes;
    unordered_set<int> seeds;
    for (auto [seed, cluster] : clusters)
        seeds.insert(seed);

    unordered_map<int, int> nd2comm;
    unordered_map<int, vector<int>> nd2cluster;
    for (auto [nd, c] : original_id_to_community)
    {
        if (c2nodes.find(c) == c2nodes.end())
            c2nodes[c] = vector<int>();
        c2nodes[c].push_back(nd);
        if (seeds.find(nd) != seeds.end())
            nd2comm[nd] = c;
    }

    for (auto [seed, c] : nd2comm)
    {
        nd2cluster[seed] = c2nodes[c];
    }

    return nd2cluster;
}

unordered_map<int, vector<int>> extract_cluster(ifstream &input)
{
    unordered_map<int, vector<int>> clusters;
    string line;
    while (getline(input, line))
    {
        vector<int> cluster;
        std::istringstream ipt(line);
        string word;
        while (ipt >> word)
        {
            int nd = stoi(word);
            cluster.push_back(nd);
        }
        int seed = cluster[0];
        clusters[seed] = vector<int>(cluster.begin() + 1, cluster.end());
    }
    return clusters;
}

void update_best_partition(
    std::unordered_map<int, std::vector<int>> &best_partition,
    std::unordered_map<int, std::vector<int>> &partition,
    std::unordered_map<int, std::vector<int>> &gt)
{
    for (auto [seed, cluster] : gt)
    {
        auto &near_sized_cluster = best_partition[seed];
        auto &louvain_partition = partition[seed];
        if (abs(int(louvain_partition.size() - cluster.size())) < abs(int(near_sized_cluster.size() - cluster.size())))
            best_partition[seed] = louvain_partition;
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    time_t time_begin, time_end;
    time(&time_begin);
    display_time("start");

    string filepath = argv[1];

    Community c(filepath, UNWEIGHTED, PRECISION);

    display_time("file read");
    // c.g.print_links();
    // c.g.print_degrees();

    double mod = c.modularity();

    cerr << "network : "
         << c.g.num_nodes << " nodes, "
         << c.g.num_links << " links, "
         << c.g.total_weight << " weight." << endl;

    double new_mod = c.one_level();
    unordered_map<int, int> original_id_to_community;
    for (auto [node, c] : c.g.original_id_to_node_id)
    {
        original_id_to_community[node] = c;
    }

    ifstream input("cluster/com-lj-all.output");
    std::unordered_map<int, std::vector<int>> clusters = extract_cluster(input);
    std::unordered_map<int, std::vector<int>> partition = get_partition(original_id_to_community, clusters);
    std::unordered_map<int, std::vector<int>> best_partition = partition;
    display_time("communities computed");
    cerr << "modularity increased from " << mod << " to " << new_mod << endl;

    if (DISPLAY_LEVEL == -1)
        c.display_partition();

    Graph g = c.partition2graph_binary();
    update_original_node_community(original_id_to_community, c.community_of, g.original_id_to_node_id);

    display_time("network of communities computed");

    int level = 0;
    while (new_mod - mod > PRECISION)
    {
        partition = get_partition(original_id_to_community, clusters);
        update_best_partition(best_partition, partition, clusters);

        mod = new_mod;
        Community c(g, PRECISION);

        cerr << "\nnetwork : "
             << c.g.num_nodes << " nodes, "
             << c.g.num_links << " links, "
             << c.g.total_weight << " weight." << endl;

        new_mod = c.one_level();

        display_time("communities computed");
        cerr << "modularity increased from " << mod << " to " << new_mod << endl;

        if (DISPLAY_LEVEL == -1)
            c.display_partition();

        g = c.partition2graph_binary();
        level++;

        update_original_node_community(original_id_to_community, c.community_of, g.original_id_to_node_id);

        if (level == DISPLAY_LEVEL)
            g.display();

        display_time("network of communities computed");
    }
    time(&time_end);

    // string output_path = "cluster/louvain-" + filepath.substr(6);
    // output_path.replace(output_path.end() - 2, output_path.end(), "output");
    // string output_path = "cluster/louvain-com-lj.ungraph.output";
    string output_path = "cluster/louvain-com-lj.all.output";
    cout << output_path << endl;
    ofstream output(output_path);
    // print_partitions(partitions);

    for (auto [seed, cluster] : clusters)
    {
        output << seed;
        for (auto nd : best_partition[seed])
            output << " " << nd;
        output << endl;
    }

    cerr << PRECISION << " " << new_mod << " " << (time_end - time_begin) << endl;
}
