#include "graph.hpp"

template <class T>
Graph<T>::Graph() {
	this->vertices = new std::vector<Node<T>>();
}

template <class T>
Graph<T>::addVertex(Node<T> & v) {
	this->vertices.push_back(v);
}

template <class T>
Graph<T>::addEdge(Node<T> * v, Node<T> * u) {
	//todo: check that u and v are in vertices
	this->addDirectedEdge(v, u);
	this->addDirectedEdge(u, v);
}

template <class T>
Graph<T>::addDirectedEdge(Node<T> * v, Node<T> * u) {
	//todo: check that u and v are in vertices
	v->edges->push_back(u);
}

template <class T>
Node<T>::Node(T item, int n, std::vector<Node<T>> & edges){
	this->data = item;
	this->edges = edges;
}

template <class T>
Node<T>::Node(T item, int n, Node<T> & edges) {
	this->data = item;
	this->edges = new std::vector<Node<T>>(n);
	for (int i = 0; i < n; i++)
		this->edges[i] = edges[i];
}
