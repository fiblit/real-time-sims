#include "PRM.hpp"

/* uniformly samples the configuration space to generate nodes for the PRM;
   All sampled points will be non-colliding with the static environment 
*/
VecPoint * PRM::sampleNodes(Cspace_2D * cSpace_) {
	typedef std::chrono::high_resolution_clock hrclock;
	hrclock::time_point first = hrclock::now();

	std::default_random_engine gen;
	std::uniform_real_distribution<float> std(-0.5f, 0.5f);
	const int samplecount = 5;//this does not work on all maps

	hrclock::duration seed = hrclock::now() - first;
	gen.seed(static_cast<unsigned int>(seed.count()));

	float b = 2.8f;//bin size

	VecPoint * sample = new VecPoint();
	for (int i = 0; i < samplecount; i++) {
		for (float x = -10+b/2; x < 10-b/2; x+=b) {
			for (float y = -10+b/2; y < 10-b/2; y+=b) {
				Point p;
				p.x = std(gen)*b + x;
				p.y = std(gen)*b + y;
				do {
					p.x += std(gen)/5;
					p.y += std(gen)/5;
					p.x = std::min(std::max(p.x, -10.f), 10.f);
					p.y = std::min(std::max(p.y, -10.f), 10.f);
				} while (cSpace_->isCollision(p));
				sample->push_back(new Node<Point>(p, new VecPoint()));
				//else
				//	i--;
			}
		}
	}

	return sample;
}

/* threshold search to find NNs */
VecPoint * PRM::findNearestNeighbours(VecPoint * nodes, int targetIdx) {
	float threshold = 3.f; // METERS

	VecPoint * neighbours = new VecPoint();

	Point t = (*nodes)[targetIdx]->data;
	for (int i = 0; i < nodes->size(); i++) {
		Point n = (*nodes)[i]->data;
		// don't consider this node we're looking from
		if (i != targetIdx)
			if (distP(t, n) < threshold)
				// push the close enough node onto the neighbours list
				neighbours->push_back((*nodes)[i]);
	}

	return neighbours;
}

/* connects NNs of each node by Graph edges */
Graph<Point> * PRM::connectRoadmap(VecPoint * nodes) {
	Graph<Point> * G = new Graph<Point>();
	for (int i = 0; i < nodes->size(); i++)
		G->addVertex((*nodes)[i]);

	for (int i = 0; i < nodes->size(); i++) {

		VecPoint * NNs = findNearestNeighbours(nodes, i);

		for (int n = 0; n < NNs->size(); n++) {
			if (this->cSpace->lineOfSight((*NNs)[n]->data, (*nodes)[i]->data)) {
				// we want directed because we'll be passing over the other side during
				// the course of the outer loop
				G->addDirectedEdge((*NNs)[n], (*nodes)[i]);
			}
		}
	}

	return G;
}

/* samples and connects a Pobabilistic Road Map */
PRM::PRM(Point start, Point goal, Cspace_2D * cSpace) {
	this->cSpace = cSpace;

	Node<Point> * startNode = new Node<Point>(start, new VecPoint());
	Node<Point> * goalNode = new Node<Point>(goal, new VecPoint());
	
	VecPoint * sample = sampleNodes(this->cSpace);
	sample->insert(sample->begin(), goalNode);
	sample->insert(sample->begin(), startNode);
	
	this->roadmap = connectRoadmap(sample);
}

/* generates a configuartion space given a list of obstacles (circles-only) and agents (circle-only; 1-only) */
Cspace_2D::Cspace_2D(std::vector<Circle> cobs_, std::vector<Rect> robs_, Circle * cagent, Rect * ragent) {
	init(cobs_.data(), static_cast<int>(cobs_.size()), robs_.data(), static_cast<unsigned int>(robs_.size()), cagent, ragent);
}

Cspace_2D::Cspace_2D(Circle * cobs_, int cn, Rect * robs_, int rn, Circle * cagent, Rect * ragent) {
	init(cobs_, cn, robs_, rn, cagent, ragent);
}

void Cspace_2D::init(Circle * cobs_, int cn, Rect * robs_, int rn, Circle * cagent, Rect * ragent) {
	this->cobs = new std::vector<Circle>();
	this->robs = new std::vector<Rect>();
	for (int i = 0; i < cn; i++) {
		if (cagent != nullptr) {
			Circle c;
			c.r = cobs_[i].r + cagent->r;
			c.o = cobs_[i].o;
			this->cobs->push_back(c);
		}
		else if (ragent != nullptr) {
			// add robs
			Rect r;
			r.o = cobs_[i].o;
			r.w = ragent->w;
			r.h = 2*cobs_[i].r + ragent->h;
			this->robs->push_back(r);
			r.w = 2*cobs_[i].r + ragent->w;
			r.h = ragent->h;
			this->robs->push_back(r);

			// add cobs
			Circle c;
			c.r = cobs_[i].r;
			c.o.x = cobs_[i].o.x + ragent->w / 2;
			c.o.y = cobs_[i].o.y + ragent->h / 2;
			this->cobs->push_back(c);

			c.o.x = cobs_[i].o.x - ragent->w / 2;
			c.o.y = cobs_[i].o.y + ragent->h / 2;
			this->cobs->push_back(c);

			c.o.x = cobs_[i].o.x + ragent->w / 2;
			c.o.y = cobs_[i].o.y - ragent->h / 2;
			this->cobs->push_back(c);

			c.o.x = cobs_[i].o.x - ragent->w / 2;
			c.o.y = cobs_[i].o.y - ragent->h / 2;
			this->cobs->push_back(c);
		}
	}
	for (int i = 0; i < rn; i++) {
		if (ragent != nullptr) {
			Rect r;
			r.w = robs_[i].w + ragent->w;
			r.h = robs_[i].h + ragent->h;
			r.o = robs_[i].o;
			this->robs->push_back(r);
		}
		else if (cagent != nullptr) {
			// add robs
			Rect r;
			r.o = robs_[i].o;
			r.w = robs_[i].w;
			r.h = 2 * cagent->r + robs_[i].h;
			this->robs->push_back(r);
			r.w = 2 * cagent->r + robs_[i].w;
			r.h = robs_[i].h;
			this->robs->push_back(r);

			// add cobs
			Circle c;
			c.r = cagent->r;
			c.o.x = robs_[i].o.x + robs_[i].w / 2;
			c.o.y = robs_[i].o.y + robs_[i].h / 2;
			this->cobs->push_back(c);

			c.o.x = robs_[i].o.x - robs_[i].w / 2;
			c.o.y = robs_[i].o.y + robs_[i].h / 2;
			this->cobs->push_back(c);

			c.o.x = robs_[i].o.x + robs_[i].w / 2;
			c.o.y = robs_[i].o.y - robs_[i].h / 2;
			this->cobs->push_back(c);

			c.o.x = robs_[i].o.x - robs_[i].w / 2;
			c.o.y = robs_[i].o.y - robs_[i].h / 2;
			this->cobs->push_back(c);
		}
	}
}

/* detects if a point collides with anything in the configuration space */
bool Cspace_2D::isCollision(Point p) {
	for (int i = 0; i < this->cobs->size(); i++) {
		Point o = (*this->cobs)[i].o;
		Point diff = subP(p, o);
		if (dotP(diff, diff) <= (*this->cobs)[i].r * (*this->cobs)[i].r)
			return true;
	}
	for (int i = 0; i < this->robs->size(); i++) {
		Rect r;
		r = (*this->robs)[i];
		if (abs(p.x - r.o.x) <= r.w /2 
				&& abs(p.y - r.o.y) <= r.h / 2)
			return true;
	}
	return false;
}

/* just some basic vector math */
/* TODO: change to glm.... why didn't I do that in the first place? */
float dotP(Point a, Point b) {
	return a.x * b.x + a.y * b.y;
}
float distP(Point a, Point b) {
	Point d = subP(a, b);
	return sqrt(dotP(d, d));
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

void printP(Point a, std::string s = "P") {
	std::cout << (std::string)s + (std::string)" x" + (std::string)std::to_string(a.x) + (std::string)" y" + (std::string)std::to_string(a.y) << std::endl;
}

/* projective geometry solution. I learned this from computer vision. :) */
/* sadly it is broken */
bool lineSegCollision(Point p1, Point p2, Point p3, Point p4) {
	Point pp[4] = { p1, p2, p3, p4 };
	glm::vec3 l[2], p[4], x;

	for (int i = 0; i < 4; i++)
		p[i] = glm::vec3(pp[i].x, pp[i].y, 1);

	l[0] = glm::cross(p[0], p[1]);
	l[1] = glm::cross(p[2], p[3]);
	x = glm::cross(l[0], l[1]);
	Point px;
	px.x = x.x / x.z;
	px.y = x.y / x.z;

	float len2l1 = (l[0].y * l[0].y) + (l[0].x * l[0].x);
	float len2l2 = (l[1].y * l[1].y) + (l[1].x * l[1].x);

	Point pp2x[4];
	for (int i = 0; i < 4; i++)
		pp2x[i] = subP(px, pp[i]);
	for (int i = 0; i < 2; i++)
		if (dotP(pp2x[i], pp2x[i]) > len2l1)
			return false;//miss
	for (int i = 3; i < 5; i++)
		if (dotP(pp2x[i], pp2x[i]) > len2l2)
			return false;//miss

	return true;//must have hit
}

bool axialLineSegLineSegCollision(Point pp1, Point pp2, float val, int axis, float oValLo, float oValHi) {
	glm::vec3 l, p1, p2;
	p1 = glm::vec3(pp1.x, pp1.y, 1);
	p2 = glm::vec3(pp2.x, pp2.y, 1);
	l = glm::cross(p1, p2);
	//vertical
	if (axis == 0) {// (1/val)*x + 0*y - 1 = 0 // x = val
		float yint = (-l[0] * val - l[2]) / l[1];
		if (((pp1.x <= val && val <= pp2.x) || (pp2.x <= val && val <= pp1.x))//val line hits lineseg
			&& (oValLo <= yint && yint <= oValHi)) { //intersection on axial segment
			return true;
		}
	}
	//horizontal
	else if (axis == 1) {// 0x + (1/val)*y - 1 = 0 // y =val
		float xint = (-l[1] * val - l[2]) / l[0];
		if (((pp1.y <= val && val <= pp2.y) || (pp2.y <= val && val <= pp1.y))//axis line hits lineseg
			&& (oValLo <= xint && xint <= oValHi)) { //intersection on axial segment
			return true;
		}
	}
	return false;

}

/* detects if a line segment between Point a and Point b collides with the C-space 
 */
bool Cspace_2D::lineOfSight(Point a, Point b) {
	Point Lab;
	Lab.x = b.x - a.x;
	Lab.y = b.y - a.y;
	float len2 = dotP(Lab, Lab);

	for (int i = 0; i < this->cobs->size(); i++) {
		Circle c;
		c = this->cobs->at(i);

		Point Lao;
		Lao.x = c.o.x - a.x;
		Lao.y = c.o.y - a.y;

		float r2 = c.r * c.r;
		if (dotP(Lao, Lao) <= r2) //point a inside circle
			return false; // HIT

		Point Lbo;
		Lbo.x = c.o.x - b.x;
		Lbo.y = c.o.y - b.y;
		if (dotP(Lbo, Lbo) <= r2) //point b inside circle
			return false; // HIT

		float ang = dotP(Lab, Lao);
		Point proj = scaleP(Lab, ang / len2);
		Point rej = subP(Lao, proj);
		float plen2 = dotP(proj, proj);

		if (dotP(rej, rej) <= r2  //close enough tangentially
				&& 0 <= ang       //point a before circle center
				&& plen2 <= len2) //point b after circle center
			return false; // HIT
	}

	for (int i = 0; i < this->robs->size(); i++) {
		Rect r = this->robs->at(i);
		float left   = r.o.x - r.w / 2;
		float right  = r.o.x + r.w / 2;
		float top    = r.o.y + r.h / 2;
		float bottom = r.o.y - r.h / 2;

		if (axialLineSegLineSegCollision(a, b, left, 0, bottom, top)
				|| axialLineSegLineSegCollision(a, b, right, 0, bottom, top)
				|| axialLineSegLineSegCollision(a, b, bottom, 1, left, right)
				|| axialLineSegLineSegCollision(a, b, top, 1, left, right))
			return false;
	}

	return true; // MISS
}
