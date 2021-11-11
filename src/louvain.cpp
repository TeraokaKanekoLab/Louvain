#include "community.cpp"

#define PRECISION 0.000001
#define DISPLAY_LEVEL -2

void display_time(const char* str)
{
    time_t rawtime;
    time(&rawtime);
    // cout << str << " : " << ctime(&rawtime);
}

void update_original_node_community(unordered_map<int, int>& original_id_to_community, vector<int>& community_of, unordered_map<int, int>& renum)
{
    for (auto [node, c] : original_id_to_community) {
        int community = community_of[c];
        int new_community = renum[community];
        original_id_to_community[node] = new_community;
    }
}

pair<int, int> louvain(string path, int num_merged)
{
    srand(time(NULL));

    time_t time_begin, time_end;
    time(&time_begin);
    display_time("start");

    string filepath = path;

    Community c(filepath, UNWEIGHTED, PRECISION);

    display_time("file read");
    // c.g.print_links();
    // c.g.print_degrees();

    double mod = c.modularity();

    // cout << "network : "
    //      << c.g.num_nodes << " nodes, "
    //      << c.g.num_links << " links, "
    //      << c.g.total_weight << " weight." << endl;

    double new_mod = c.one_level();
    unordered_map<int, int> original_id_to_community;
    for (auto [node, c] : c.g.original_id_to_node_id) {
        original_id_to_community[node] = c;
    }

    display_time("communities computed");
    // cout << "modularity increased from " << mod << " to " << new_mod << endl;

    if (DISPLAY_LEVEL == -1)
        c.display_partition();

    Graph g = c.partition2graph_binary();
    update_original_node_community(original_id_to_community, c.community_of, g.original_id_to_node_id);

    display_time("network of communities computed");

    int level = 0;
    int num_communities = 0;
    while (new_mod - mod > PRECISION) {
        mod = new_mod;
        Community c(g, PRECISION);
        num_communities = c.g.num_nodes;
        // cout << "\nnetwork : "
        //      << c.g.num_nodes << " nodes, "
        //      << c.g.num_links << " links, "
        //      << c.g.total_weight << " weight." << endl;

        new_mod = c.one_level();

        display_time("communities computed");
        // cout << "modularity increased from " << mod << " to " << new_mod << endl;

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

    string output_path = "community/" + filepath.substr(6);
    output_path.replace(output_path.end() - 2, output_path.end(), "cm");
    // cout << output_path << endl;
    ofstream output(output_path);

    for (auto [original, community] : original_id_to_community) {
        output << original << " " << community << endl;
    }

    // cout << PRECISION << " " << new_mod << " " << (time_end - time_begin) << endl;

    // if (num_communities == 6)
    //     return 0;
    vector<int> num_nodes_in_communities(num_communities, 0);
    for (auto [original, community] : original_id_to_community) {
        num_nodes_in_communities[community]++;
    }
    // cout << ", [" << num_nodes_in_communities[0];
    // for (int i = 1; i < num_communities; ++i) {
    //     cout << ", " << num_nodes_in_communities[i];
    // }
    // cout << "]" << endl;
    int c1_took = 100 - num_nodes_in_communities[3];
    int c4_took = 100 - num_nodes_in_communities[0];
    int joined = c1_took + c4_took;
    if (joined != num_merged)
        return make_pair(-1, -1);
    if (num_communities != 6)
        return make_pair(-1, -1);
    if (num_nodes_in_communities[1] != 100 || num_nodes_in_communities[2] != 100 || num_nodes_in_communities[4] != 100 || num_nodes_in_communities[5] != 100)
        return make_pair(-1, -1);
    // cout << num_communities << "," << new_mod << ",";
    // cout << "the dense took: " << dense_took << "/" << joined << " (" << (double)dense_took / joined << ")" << endl;

    return make_pair(c1_took, c4_took);
}

void loop()
{
    string path = "graph/merged.gr";
    vector<int> num_joined_s = vector<int> { 5, 10, 15, 20, 25, 30, 35, 40, 45, 50 };
    ofstream output("output/result.csv");

    for (int k = 30; k < 45; k += 5) {
        double density1 = (double)k / 100;
        string statement = "python3 src/sbm1.py " + to_string(density1);
        const char* c = statement.c_str();
        system(c);
        output << "sparse density: " << density1 << endl;
        output << "edge density";
        for (int l = 20; l <= 50; l += 1)
            output << "," << double(l) / 100;
        output << endl;
        for (int i : num_joined_s) {
            output << i;
            for (int l = 20; l <= 50; l += 1) {
                double density2 = (double)l / 100;
                statement = "python3 src/sbm2.py " + to_string(density2);
                c = statement.c_str();
                system(c);
                int sum1 = 0;
                int sum2 = 0;
                int cnt = 0;
                for (int j = 0; j < 100; ++j) {
                    statement = "python3 src/merge.py graph/sbm1.gr graph/sbm2.gr " + to_string(i);
                    c = statement.c_str();
                    system(c);
                    pair<int, int> c1_c4 = louvain(path, i);
                    int c1_took = c1_c4.first;
                    int c4_took = c1_c4.second;
                    if (c1_took < 0 || c4_took < 0)
                        continue;
                    ++cnt;
                    sum1 += c1_took;
                    sum2 += c4_took;
                }
                if (cnt == 0) {
                    // cerr << "counted 0" << endl;
                    break;
                }
                int total = cnt * i;
                // output << "joined: " << i << ", count: " << cnt << ", total: " << total << ", count1: " << sum1 << " (" << (double)sum1 / total << "), count4: " << sum2 << " (" << (double)sum2 / total << ")" << endl;
                // output << i << "," << cnt << "," << total << "," << sum1 << "," << (double)sum1 / total << "," << sum2 << "," << (double)sum2 / total << endl;
                output << "," << (double)sum2 / total;
            }
            output << endl;
        }
    }
}

int main(int argc, char** argv)
{
    loop();
    // louvain(argv[1]);
}
