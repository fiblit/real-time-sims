#ifndef GRAPH_H_GUARD
#define GRAPH_H_GUARD

#include <vector>

template <class T>
class Graph {
private:
	std::vector<Node<T>> * vertices;

public:
	Graph();
	void addVertex(Node<T> & v);
	void addEdge(Node<T> & v, Node<T> & u);
};

template <class T>
class Node {
	T data;
	std::vector<Node> * edges;

	Node(T item, int n, std::vector<Node<T>> & edges);
	Node(T item, int n, Node<T> & edges);
};

#endif // GRAPH_H_GUARD
