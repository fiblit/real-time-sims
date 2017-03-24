#include "PRM.hpp"

/* uniformly samples the configuration space to generate nodes for the PRM;
   All sampled points will be non-colliding with the static environment 
*/
VecPoint * PRM::sampleNodes(Cspace_2D * cSpace) {
	typedef std::chrono::high_resolution_clock hrclock;
	hrclock::time_point first = hrclock::now();

	std::default_random_engine gen;
	std::uniform_real_distribution<float> xrand(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yrand(-10.0f, 10.0f);
	const int samplecount = 300;

	hrclock::duration seed = hrclock::now() - first;
	gen.seed(seed.count());

	VecPoint * sample = new VecPoint();
	for (int i = 0; i < samplecount; i++) {
		Point p;
		p.x = xrand(gen);
		p.y = yrand(gen);

		if (!this->cSpace->isCollision(p))
			sample->push_back( new Node<Point>(p, new VecPoint()) );
		//else
		//	i--;
	}

	return sample;
}

/* threshold search to find NNs */
VecPoint * PRM::findNearestNeighbours(VecPoint * nodes, int targetIdx) {
	int threshold = 5.0f; // 5m radius(?)

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

/* custom uniform cost search (aka Djikstra's search) for a PRM Graph 
simplifciation of A*. (h = 0)
*/
VecPoint * PRM::findPathUCS() {
	// maximum g-cost
	const int maxi = std::numeric_limits<int>::max();

	/* typedefs for readability */
	typedef Node<Point> * Vert;
	typedef std::unordered_set<Vert> Set;
	typedef std::unordered_map<Vert, Vert> VertVert;
	typedef std::unordered_map<Vert, float> VertFloat;

	// parent tree
	VertVert parents = VertVert();
	VertFloat gcost = VertFloat();
	
	//initialize
	VecPoint verts = *(this->roadmap->vertices);

	Vert start = verts[0];
	Vert target = verts[1];
	gcost[start] = 0.0f;
	parents[start] = nullptr;
	
	//skip start; i = 0
	for (int i = 1; i < verts.size(); i++) {
		Vert v = verts[i];
		parents[v] = nullptr;
		gcost[v] = maxi;
	}

	// closed set
	Set closed = Set();

	//create PQ
	auto cmp = [gcost](Node<Point> * l, Node<Point> * r) { return gcost.at(l) > gcost.at(r); }; //normally <
	std::priority_queue<Vert, std::vector<Vert>, decltype(cmp)> pq(cmp);
	pq.push(verts[0]);

	while (!pq.empty()) {
		Vert u = pq.top();
		pq.pop();

		//add to closed
		closed.insert(u);

		for (int e = 0; e < u->edges->size(); e++) {
			Vert adj = (*u->edges)[e];
			if (closed.count(adj) > 0)
				continue;
			
			float g_alt = gcost[u] + distP(adj->data, u->data);
			if (g_alt < gcost[adj]) {
				gcost[adj] = g_alt;
				parents[adj] = u;

				std::vector<Vert> pqvec = std::vector<Vert>();
				//inefficient as hell; only way to fix is to write my own PQ; not in the mood to do that rn at 4 AM
				// ACTUALLY; the best way to handle this would be to just maintain a secondary set, like closed.
				//pull data out of the pq, but don't keep it off (immediately return to pq)
				while (!pq.empty()) {
					pqvec.push_back(pq.top());
					pq.pop();
				}
				for (int i = 0; i < pqvec.size(); i++) {
					pq.push(pqvec[i]);
				}

				// update adj if it is already on the PQ
				if (std::any_of(pqvec.begin(), pqvec.end(), [adj](Vert v) {return v == adj; })) {
					while (!pq.empty()) {
						pq.pop();
					}
					while (pqvec.size() > 0) {
						pq.push(pqvec.back());
						pqvec.pop_back();
					}
				}
				// add to PQ if not already on it
				else {
					pq.push(adj);
				}
			}
		}
		if (u == target)
			break;
	}
		
	// retrace path
	VecPoint * path = new VecPoint();
	Vert curr = target;
	bool flag = false;
	while (curr != nullptr) {
		if (curr == start)
			flag = true;
		path->insert(path->begin(), curr);
		curr = parents[curr];
	}
	if (flag)
		return path;
	else
		return new VecPoint();
}

/* custom A* search for a PRM Graph */
VecPoint * PRM::findPathAstar(float e) {
	// maximum g-cost
	const int maxi = std::numeric_limits<int>::max();

	/* typedefs for readability */
	typedef Node<Point> * Vert;
	typedef std::unordered_set<Vert> Set;
	typedef std::unordered_map<Vert, Vert> VertVert;
	typedef std::unordered_map<Vert, float> VertFloat;

	// parent tree
	VertVert parents = VertVert();
	VertFloat gcost = VertFloat();
	VertFloat hcost = VertFloat();

	//initialize
	VecPoint verts = *(this->roadmap->vertices);
	Vert start = verts[0];
	Vert target = verts[1];
	gcost[start] = 0.0f;
	hcost[start] = distP(start->data, target->data);
	parents[start] = nullptr;

	//skip start; i = 0
	//wish I didn't have to do this up front... dumb lambdas.
	for (int i = 1; i < verts.size(); i++) {
		Vert v = verts[i];
		parents[v] = nullptr;
		gcost[v] = maxi;
		hcost[v] = distP(v->data, target->data);
	}

	// closed set
	Set closed = Set();

	//create PQ
	auto cmp = [gcost, hcost, e](Node<Point> * l, Node<Point> * r) { return gcost.at(l) + e * hcost.at(l) > gcost.at(r) + e * hcost.at(r); }; //normally <
	std::priority_queue<Vert, std::vector<Vert>, decltype(cmp)> pq(cmp);
	pq.push(verts[0]);

	while (!pq.empty()) {
		Vert u = pq.top();
		pq.pop();

		// we will never change u again, so it is closed
		closed.insert(u);

		for (int e = 0; e < u->edges->size(); e++) {
			Vert adj = (*u->edges)[e];
			if (closed.count(adj) > 0)
				continue;

			//only consider g-cost since the h-cost can't be changed via a new path.
			float g_alt = gcost[u] + distP(adj->data, u->data);
			if (g_alt < gcost[adj]) {
				gcost[adj] = g_alt;
				parents[adj] = u;

				std::vector<Vert> pqvec = std::vector<Vert>();
				//inefficient as hell; only way to fix is to write my own PQ; not in the mood to do that rn at 4 AM
				// ACTUALLY; the best way to handle this would be to just maintain a secondary set, like closed.
				//pull data out of the pq, but don't keep it off (immediately return to pq)
				while (!pq.empty()) {
					pqvec.push_back(pq.top());
					pq.pop();
				}
				for (int i = 0; i < pqvec.size(); i++) {
					pq.push(pqvec[i]);
				}

				// update adj if it is already on the PQ
				if (std::any_of(pqvec.begin(), pqvec.end(), [adj](Vert v) {return v == adj; })) {
					//inefficient as hell; ...
					while (!pq.empty()) {
						pq.pop();
					}
					while (pqvec.size() > 0) {
						pq.push(pqvec.back());
						pqvec.pop_back();
					}
				}
				// add to PQ if not already on it
				else {
					pq.push(adj);
				}
			}
		}
		if (u == target)
			break;
	}

	// retrace path
	VecPoint * path = new VecPoint();
	Vert curr = target;
	bool flag = false;
	while (curr != nullptr) {
		if (curr == start)
			flag = true;
		path->insert(path->begin(), curr);
		curr = parents[curr];
	}
	if (flag)
		return path;
	else
		return new VecPoint();
}

/* generates a configuartion space given a list of obstacles (circles-only) and agents (circle-only; 1-only) */
Cspace_2D::Cspace_2D(std::vector<Circle> cobs, std::vector<Rect> robs, Circle * cagent, Rect * ragent) {
	init(cobs.data(), cobs.size(), robs.data(), robs.size(), cagent, ragent);
}

Cspace_2D::Cspace_2D(Circle * cobs, int cn, Rect * robs, int rn, Circle * cagent, Rect * ragent) {
	init(cobs, cn, robs, rn, cagent, ragent);
}

void Cspace_2D::init(Circle * cobs, int cn, Rect * robs, int rn, Circle * cagent, Rect * ragent) {
	this->cobs = new std::vector<Circle>();
	this->robs = new std::vector<Rect>();
	for (int i = 0; i < cn; i++) {
		if (cagent != nullptr) {
			Circle c;
			c.r = cobs[i].r + cagent->r;
			c.o = cobs[i].o;
			this->cobs->push_back(c);
		}
		else if (ragent != nullptr) {
			// add robs
			Rect r;
			r.o = cobs[i].o;
			r.w = ragent->w;
			r.h = 2*cobs[i].r + ragent->h;
			this->robs->push_back(r);
			r.w = 2*cobs[i].r + ragent->w;
			r.h = ragent->h;
			this->robs->push_back(r);

			// add cobs
			Circle c;
			c.r = cobs[i].r;
			c.o.x = cobs[i].o.x + ragent->w / 2;
			c.o.y = cobs[i].o.y + ragent->h / 2;
			this->cobs->push_back(c);

			c.o.x = cobs[i].o.x - ragent->w / 2;
			c.o.y = cobs[i].o.y + ragent->h / 2;
			this->cobs->push_back(c);

			c.o.x = cobs[i].o.x + ragent->w / 2;
			c.o.y = cobs[i].o.y - ragent->h / 2;
			this->cobs->push_back(c);

			c.o.x = cobs[i].o.x - ragent->w / 2;
			c.o.y = cobs[i].o.y - ragent->h / 2;
			this->cobs->push_back(c);
		}
	}
	for (int i = 0; i < rn; i++) {
		if (ragent != nullptr) {
			Rect r;
			r.w = robs[i].w + ragent->w;
			r.h = robs[i].h + ragent->h;
			r.o = robs[i].o;
			this->robs->push_back(r);
		}
		else if (cagent != nullptr) {
			// add robs
			Rect r;
			r.o = robs[i].o;
			r.w = robs[i].w;
			r.h = 2 * cagent->r + robs[i].h;
			this->robs->push_back(r);
			r.w = 2 * cagent->r + robs[i].w;
			r.h = robs[i].h;
			this->robs->push_back(r);

			// add cobs
			Circle c;
			c.r = cagent->r;
			c.o.x = robs[i].o.x + robs[i].w / 2;
			c.o.y = robs[i].o.y + robs[i].h / 2;
			this->cobs->push_back(c);

			c.o.x = robs[i].o.x - robs[i].w / 2;
			c.o.y = robs[i].o.y + robs[i].h / 2;
			this->cobs->push_back(c);

			c.o.x = robs[i].o.x + robs[i].w / 2;
			c.o.y = robs[i].o.y - robs[i].h / 2;
			this->cobs->push_back(c);

			c.o.x = robs[i].o.x - robs[i].w / 2;
			c.o.y = robs[i].o.y - robs[i].h / 2;
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
		//std::cout << "y" << pp1.x << " " << val << " " << pp2.x << " " << oValLo << " " << yint << " " << oValHi << std::endl;
		if (((pp1.x <= val && val <= pp2.x) || (pp2.x <= val && val <= pp1.x))//val line hits lineseg
			&& (oValLo <= yint && yint <= oValHi)) { //intersection on axial segment
			//std::cout << "!" << std::endl;
			return true;
		}
	}
	//horizontal
	else if (axis == 1) {// 0x + (1/val)*y - 1 = 0 // y =val
		float xint = (-l[1] * val - l[2]) / l[0];
		//std::cout << "y" << pp1.y << " " << val << " " << pp2.y << " " << oValLo << " " << xint << " " << oValHi << std::endl;
		if (((pp1.y <= val && val <= pp2.y) || (pp2.y <= val && val <= pp1.y))//axis line hits lineseg
			&& (oValLo <= xint && xint <= oValHi)) { //intersection on axial segment
			//std::cout << "!" << std::endl;
			return true;
		}
	}
	return false;

}

/* detects if a line segment between Point a and Point b collides with the C-space 
   method : projects the line (AC) onto (AB), determines if the projection is close 
   enough to the segment and if the rejection is within the circle
   I did this because it didn't involve any square roots. And I thought it was cool.
   Oh, and I was tired/delusional.
   Not even sure if it is more efficient than the classic ray-sphere, lol.
   It seems like there are more dot products, but hey! No square roots!

   If you want to play with it I made this:
   https://www.desmos.com/calculator/fxgnyi8skw
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
