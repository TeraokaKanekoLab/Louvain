import networkx as nx
import community
import sys
import matplotlib.cm as cm
import matplotlib.pyplot as plt


def louvain(graph):
    partition = community.best_partition(graph)
    print(partition)
    # draw the graph
    pos = nx.spring_layout(graph)
    # color the nodes according to their partition
    cmap = cm.get_cmap('viridis', max(partition.values()) + 1)
    nx.draw_networkx_nodes(graph, pos, partition.keys(), node_size=40,
                           cmap=cmap, node_color=list(partition.values()))
    nx.draw_networkx_edges(graph, pos, alpha=0.5)
    plt.show()


def read_graph():
    # filepath = "graph/com-amazon.gr"
    # filepath = "graph/fb-pages-food.gr"
    filepath = sys.argv[1]
    graph = nx.Graph()
    f = open(filepath, "r")
    lines = f.readlines()
    for line in lines:
        if '#' in line:
            continue
        left = int(line.split()[0])
        right = int(line.split()[1])
        if left == right:
            continue
        graph.add_edge(left, right)
        graph.add_edge(right, left)
    f.close()
    return graph


if __name__ == "__main__":
    graph = read_graph()
    louvain(graph)
