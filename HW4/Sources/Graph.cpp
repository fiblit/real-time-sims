#include "graph.hpp"
/*
// putting it in the CPP wasn't working for some reason. commented it out to compile it. Will figure this out later.
template <class T>
Graph<T>::Graph() {
	this->vertices = new std::vector<Node<T> *>();
}

template <class T>
void Graph<T>::addVertex(Node<T> * v) {
	this->vertices.push_back(v);
}

template <class T>
void Graph<T>::addEdge(Node<T> * v, Node<T> * u) {
	//todo: check that u and v are in vertices
	this->addDirectedEdge(v, u);
	this->addDirectedEdge(u, v);
}

template <class T>
void Graph<T>::addDirectedEdge(Node<T> * v, Node<T> * u) {
	//todo: check that u and v are in vertices
	v->edges->push_back(u);
}

template <class T>
Node<T>::Node(T item, std::vector<Node<T> *> * edges){
	this->data = item;
	this->edges = edges;
}

template <class T>
Node<T>::Node(T item, int n, Node<T> * edges) {
	this->data = item;
	this->edges = new std::vector<Node<T>>(n);
	for (int i = 0; i < n; i++)
		this->edges[i] = edges[i];
}
*/
