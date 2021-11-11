import sys
import networkx as nx

density = float(sys.argv[1])
sizes = [100, 100, 100]
probs = [[density, 0.02, 0.02], [0.02, density, 0.02], [0.02, 0.02, density]]
g = nx.stochastic_block_model(sizes, probs, seed=1)
H = nx.quotient_graph(g, g.graph["partition"], relabel=True)
# for v in H.nodes(data=True):
#     print(round(v[1]["density"], 3))

nx.write_edgelist(g, "graph/sbm2.gr", data=False)
