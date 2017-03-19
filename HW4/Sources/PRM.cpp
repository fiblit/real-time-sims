#include "PRM.hpp"

/* uniformly samples the configuration space to generate nodes for the PRM;
   All sampled points will be non-colliding with the static environment 
*/
VecPoint * PRM::sampleNodes(Cspace_2D * cSpace) {
	std::default_random_engine gen;
	std::uniform_real_distribution<float> xrand(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yrand(-10.0f, 10.0f);
	const int samplecount = 400;

	VecPoint * sample = new VecPoint();
	for (int i = 0; i < samplecount; i++) {
		Point p;
		p.x = xrand(gen);
		p.y = yrand(gen);

		if (!this->cSpace->isCollision(p))
			sample->push_back( new Node<Point>(p, VecPoint()) );
		//else
		//	i--;
	}
}

/* threshold search to find NNs */
VecPoint * PRM::findNearestNeighbours(VecPoint * nodes, int targetIdx) {
	const int threshold = 3.0f; // 3m radius
	VecPoint * neighbours = new VecPoint();

	Point t = (*nodes)[targetIdx]->data;
	for (int i = 0; i < nodes->size(); i++) {
		Point n = (*nodes)[i]->data;
		// don't consider this node we're looking from
		if (i != targetIdx)
			if ((t.x - n.x) * (t.x - n.x) + (t.y - n.y) * (t.y - n.y) < threshold)
				// push the close enough node onto the neighbours list
				neighbours->push_back( (*nodes)[i] );
	}
}

/* connects NNs of each node by Graph edges */
Graph<Point> * PRM::connectRoadmap(VecPoint * nodes) {
	for (int i = 0; i < nodes->size(); i++) {
		VecPoint * NNs = findNearestNeighbours(nodes, i);
		for (int n = 0; n < NNs->size(); n++) {
			if (this->cSpace->lineOfSight((*NNs)[n]->data, (*nodes)[i]->data)) {
				// we want directed because we'll be passing over the other side during
				// the course of the outer loop
				this->roadmap->addDirectedEdge((*NNs)[n], (*nodes)[i]);
			}
		}
	}
}

/* samples and connects a Pobabilistic Road Map */
PRM::PRM(Point start, Point goal, Cspace_2D * cSpace) {
	this->cSpace = cSpace;

	Node<Point> * startNode = new Node<Point>(start, VecPoint());
	Node<Point> * goalNode = new Node<Point>(goal, VecPoint());
	VecPoint * sample = sampleNodes(this->cSpace);
	sample->insert(sample->begin(), startNode);
	sample->insert(sample->begin(), goalNode);

	this->roadmap = connectRoadmap(sample);
}

/* custom uniform cost search for a PRM Graph */
VecPoint * PRM::findPathUCS() {

	return nullptr;
}

/* custom A* search for a PRM Graph */
VecPoint * PRM::findPathAstar() {

	return nullptr;
}

/* generates a configuartion space given a list of obstacles (circles-only) and agents (circle-only; 1-only) */
Cspace_2D::Cspace_2D(std::vector<Circle> & obs, Circle & agent) {
	this->obs_circle = new std::vector<Circle>();
	for (int i = 0; i < obs.size(); i++) {
		Circle c;
		c.r = obs[i].r + agent.r;
		c.o.x = obs[i].o.x;
		c.o.y = obs[i].o.y;
		this->obs_circle->push_back(c);
	}
}

/* detects if a point collides with anything in the configuration space */
bool Cspace_2D::isCollision(Point p) {
	for (int i = 0; i < this->obs_circle->size(); i++) {
		float ox = (*this->obs_circle)[i].o.x;
		float oy = (*this->obs_circle)[i].o.y;
		float or = (*this->obs_circle)[i].r;
		if ((p.x - ox) * (p.x - ox) + (p.y - oy) * (p.y - oy) <= or * or)
			return true;
	}
	return false;
}

/* just some basic vector math */
float dotP(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}
Point addP(Point a, Point b) {
	Point c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}
Point scaleP(Point a, float s) {
	Point c;
	c.x = a.x * s;
	c.y = a.y * s;
	return c;
}
Point subP(Point a, Point b) {
	Point c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	return c;
}

/* detects if a line segment between Point a and Point b collides with the C-space 
   method : projects the line (AC) onto (AB), determines if the projection is close 
   enough to the segment and if the rejection is within the circle
   I did this because it didn't involve any square roots. And I thought it was cool.
   Oh, and I was tired/delusional.
   Not even sure if it is more efficient than the classic ray-sphere, lol.
   It seems like there are more dot products, but hey! No square roots!

   If you want to play with it:
   https://www.desmos.com/calculator/xnitdx0vw7
 */
bool Cspace_2D::lineOfSight(Point a, Point b) {
	Point Lab;
	Lab.x = b.x - a.x;
	Lab.y = b.y - a.y;
	float d = dotP(Lab, Lab);

	for (int i = 0; i < this->obs_circle->size(); i++) {
		Circle c;
		Point Lao;
		Lao.x = c.o.x - a.x;
		Lao.y = c.o.y - a.y;
		Point Lbo;
		Lbo.x = c.o.x - b.x;
		Lbo.y = c.o.y - b.y;

		float r2 = c.r * c.r;
		if (dotP(Lao, Lao) <= r2) //point a inside circle
			return false; // HIT
		if (dotP(Lbo, Lbo) <= r2) //point b inside circle
			return false; // HIT

		float n = dotP(Lab, Lao);
		Point proj = scaleP(a, n / d);
		Point rej = subP(b, proj);
		float dp = dotP(proj, proj);

		if (dotP(rej, rej) <= r2 //close enough tangentially
				&& 0 <= n        //point a before circle center
				&& dp <= d)      //point b after circle center
			return false; // HIT
	}
	return true; // MISS
}
