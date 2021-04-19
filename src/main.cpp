#include "community.cpp"

#define PRECISION 0.000001
#define DISPLAY_LEVEL -2

void display_time(const char* str)
{
    time_t rawtime;
    time(&rawtime);
    cerr << str << " : " << ctime(&rawtime);
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    time_t time_begin, time_end;
    time(&time_begin);
    display_time("start");

    Community c(argv[1], UNWEIGHTED, PRECISION);

    display_time("file read");
    // c.g.print_links();
    // c.g.print_degrees();

    double mod = c.modularity();

    cerr << "network : "
         << c.g.num_nodes << " nodes, "
         << c.g.num_links << " links, "
         << c.g.total_weight << " weight." << endl;

    double new_mod = c.one_level();

    display_time("communities computed");
    cerr << "modularity increased from " << mod << " to " << new_mod << endl;

    if (DISPLAY_LEVEL == -1)
        c.display_partition();

    Graph g = c.partition2graph_binary();

    display_time("network of communities computed");

    int level = 0;
    while (new_mod - mod > PRECISION) {
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

        if (level == DISPLAY_LEVEL)
            g.display();

        display_time("network of communities computed");
    }
    time(&time_end);

    cerr << PRECISION << " " << new_mod << " " << (time_end - time_begin) << endl;
}
