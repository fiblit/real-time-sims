#ifndef PRM_H_GUARD
#define PRM_H_GUARD

#include "graph.hpp"

template <class C>
class PRM {
private:
	Graph<C> * roadmap;
	Node<C> * start;
	Node<C> * goal;
	ConfigSpace<C> * cSpace;
public:
	PRM(C * start, C * goal, ConfigSpace<C> * cSpace);
};

#endif // PRM_H_GUARD