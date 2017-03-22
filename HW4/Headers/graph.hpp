#ifndef GRAPH_H_GUARD
#define GRAPH_H_GUARD

#include <vector>

template <class T>
class Node {
private:
public:
	T data;
	/* TODO: change to unordered_set for fast lookup, yet easy ---iteration--- <nope> */
	std::vector<Node<T> *> * edges;
	Node(T item, std::vector<Node<T> *> * edges) {
		this->data = item;
		this->edges = edges;
	};
	Node(T item, int n, Node<T> * edges) {
		this->data = item;
		this->edges = new std::vector<Node<T>>(n);
		for (int i = 0; i < n; i++)
			this->edges[i] = edges[i];
	}
	;
};

template <class T>
class Graph {
private:
public:
	/* TODO: change to unordered_set for fast lookup, yet easy  ---iteration--- <nope> */
	std::vector<Node<T> *> * vertices;
	Graph() {
		this->vertices = new std::vector<Node<T> *>();
	};
	void addVertex(Node<T> * v) {
		this->vertices->push_back(v);
	}
	;
	void addDirectedEdge(Node<T> * v, Node<T> * u) {
		//todo: check that u and v are in vertices
		v->edges->push_back(u);
	};
	void addEdge(Node<T> * v, Node<T> * u) {
		//todo: check that u and v are in vertices
		this->addDirectedEdge(v, u);
		this->addDirectedEdge(u, v);
	};
};

#endif // GRAPH_H_GUARD
