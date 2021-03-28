import numpy as np
import matplotlib.pyplot as plt
import networkx as nx

with open("results.txt", 'r') as file:
    name, count, score = file.readline()[:-1].split(" ")
    path = file.readfile()[:-1].split(" ")

    count = int(count)

    nodes = {}
    for i in range(count):
        id, x, y = map(int, file.readline()[:-1].split(" "))
        nodes.update({id: (x, y)})

    edges = []
    for i in range(count - 1):
        edges.append((list(nodes.keys())[i], list(nodes.keys())[i+1]))
    edges.append((list(nodes.keys())[-1], list(nodes.keys())[0]))

    g = nx.Graph()
    g.add_nodes_from(nodes.keys())
    g.add_edges_from(edges)
    fig, ax = plt.subplots(figsize=(12, 7))
    nx.draw(g, nodes, node_size=30, node_color='b', ax=ax, with_labels=False)

    plt.title(f"Name: {name} | Current score: {score} | Nodes count: {count}")
    plt.show()
