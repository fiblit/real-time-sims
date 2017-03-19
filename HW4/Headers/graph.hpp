#ifndef GRAPH_H_GUARD
#define GRAPH_H_GUARD

#include <vector>

template <class T>
class Graph {
private:
public:
	/* TODO: change to unordered_set for fast lookup, yet easy iteration */
	std::vector<Node<T> *> * vertices;
	Graph();
	void addVertex(Node<T> * v);
	void addDirectedEdge(Node<T> * v, Node<T> * u);
	void addEdge(Node<T> * v, Node<T> * u);
};

template <class T>
class Node {
private:
public:
	T data;
	/* TODO: change to unordered_set for fast lookup, yet easy iteration */
	std::vector<Node<T> *> * edges;
	Node(T item, std::vector<Node<T> *> & edges);
	Node(T item, int n, Node<T> * edges);
};

#endif // GRAPH_H_GUARD
