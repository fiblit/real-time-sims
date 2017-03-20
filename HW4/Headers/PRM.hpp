#ifndef PRM_H_GUARD
#define PRM_H_GUARD

#include "graph.hpp"
#include <random>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>

/*
class Extent {
	

};

struct rect {
	float x;
	float y;
	float w;
	float h;
};
*/

// aka configuration
struct Point {
	float x;
	float y;
};
struct Circle {
	Point o;
	float r;
};

typedef std::vector<Node<Point> *> VecPoint;

class PRM {
private:
	Cspace_2D * cSpace;
	VecPoint * sampleNodes(Cspace_2D * cSpace);
	VecPoint * findNearestNeighbours(VecPoint * nodes, int targetIdx);
	Graph<Point> * connectRoadmap(VecPoint * nodes);
public:
	Graph<Point> * roadmap;
	PRM(Point start, Point goal, Cspace_2D * cSpace);
	VecPoint * findPathUCS();
	VecPoint * findPathAstar();
};

class Cspace_2D {
private:
	std::vector<Circle> * obs_circle;
	void init(Circle * obs, int n, Circle & agent);
public:
	Cspace_2D(std::vector<Circle> & obs, Circle & agent);
	Cspace_2D(Circle * obs, int n, Circle & agent);
	bool isCollision(Point a);
	bool lineOfSight(Point a, Point b);
};

#endif // PRM_H_GUARD