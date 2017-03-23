#ifndef PRM_H_GUARD
#define PRM_H_GUARD

#include "graph.hpp"
#include <random>
#include <chrono>
#include <utility>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>

struct Rect {
	Point o;
	float w;
	float h;
};

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

class Cspace_2D {
private:
	std::vector<Circle> * cobs;
	std::vector<Rect> * robs;		
	void init(Circle * cobs, int cn, Rect * robs, int rn, Rect * ragent, Circle * cagent);
public:
	Cspace_2D(std::vector<Circle> cobs, std::vector<Rect> robs, Rect * ragent, Circle * cagent);
	Cspace_2D(Circle * cobs, int cn, Rect * robs, int rn, Rect * ragent, Circle * cagent);
	bool isCollision(Point a);
	bool lineOfSight(Point a, Point b);
};

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
	VecPoint * findPathAstar(float e);
};


float dotP(Point a, Point b);
float distP(Point a, Point b);
Point subP(Point a, Point b);
Point addP(Point a, Point b);
Point scaleP(Point a, float s);


#endif // PRM_H_GUARD