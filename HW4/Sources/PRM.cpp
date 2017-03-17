#include "PRM.hpp"

template <class C>
PRM<C>::PRM(C * start, C * goal, ConfigSpace<C> * cSpace) {
	roadMap = new Graph();
	this->start = start;
	this->goal = goal;
	this->cSpace = cSpace;
}