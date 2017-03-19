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
/* aka configuratio n*/
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
	Graph<Point> * roadmap;
	Cspace_2D * cSpace;
	VecPoint * sampleNodes(Cspace_2D * cSpace);
	VecPoint * findNearestNeighbours(VecPoint * nodes, int targetIdx);
	Graph<Point> * connectRoadmap(VecPoint * nodes);
public:
	PRM(Point start, Point goal, Cspace_2D * cSpace);
	VecPoint * findPathUCS();
	VecPoint * findPathAstar();
};

class Cspace_2D {
private:
	std::vector<Circle> * obs_circle;
public:
	Cspace_2D(std::vector<Circle> & obs, Circle & agent);
	bool isCollision(Point a);
	bool lineOfSight(Point a, Point b);
};

// very general
// config = 
// cspace = HashMap<name, Range> or Range[dim]
// Range:
//	int bins;
//	float axis[bins]
//  float sample();
//  int val2bin();

//or
// Range:
//   float lo;
//   float hi;
//   float sample();

#endif // PRM_H_GUARD