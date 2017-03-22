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
	const int samplecount = 0;

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
	int threshold = 80.0f; // 7m radius

	VecPoint * neighbours = new VecPoint();
	//do {

		Point t = (*nodes)[targetIdx]->data;
		for (int i = 0; i < nodes->size(); i++) {
			Point n = (*nodes)[i]->data;
			// don't consider this node we're looking from
			if (i != targetIdx)
				if (distP(t, n) < threshold)
					// push the close enough node onto the neighbours list
					neighbours->push_back((*nodes)[i]);
		}

	//} while (neighbours->size() == 0);

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
	//for (int i = 0; i < verts.size(); i++) {
		//std::cout << "v" << (i) << " es" << verts[i]->edges->size() << "\t" << verts[i] << std::endl;
	//	for (int e = 0; e < verts[i]->edges->size(); e++)
			//std::cout << "\t e" << e << " " << (*verts[i]->edges)[e] << std::endl;
	//}
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

		//std::cout << "s" << u->edges->size() << "u" << u << std::endl;
		for (int e = 0; e < u->edges->size(); e++) {
			Vert adj = (*u->edges)[e];
			if (closed.count(adj) > 0) {
				//std::cout << "closed" << std::endl;
				continue;
			}
			
			float g_alt = gcost[u] + distP(adj->data, u->data);
			//std::cout << "alt:" << g_alt << " old:" << gcost[adj] << std::endl;
			if (g_alt < gcost[adj]) {
				gcost[adj] = g_alt;
				parents[adj] = u;

				std::vector<Vert> pqvec = std::vector<Vert>();
				while (!pq.empty()) {
					pqvec.push_back(pq.top());
					pq.pop();
				}
				for (int i = 0; i < pqvec.size(); i++) {
					pq.push(pqvec[i]);
				}

				if (std::any_of(pqvec.begin(), pqvec.end(), [adj](Vert v) {return v == adj; })) {
					while (!pq.empty()) {
						pq.pop();
					}
					while (pqvec.size() > 0) {
						pq.push(pqvec.back());
						pqvec.pop_back();
					}
				}
				else {
					//std::cout << "!!!" << std::endl;
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
	while (curr != nullptr) {
		path->insert(path->begin(), curr);
		curr = parents[curr];
	}
	
	return path;
}

/* custom A* search for a PRM Graph
TODO: make this A*, not Djikstra's
*/
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
				while (!pq.empty()) {
					pqvec.push_back(pq.top());
					pq.pop();
				}
				for (int i = 0; i < pqvec.size(); i++) {
					pq.push(pqvec[i]);
				}

				if (std::any_of(pqvec.begin(), pqvec.end(), [adj](Vert v) {return v == adj; })) {
					while (!pq.empty()) {
						pq.pop();
					}
					while (pqvec.size() > 0) {
						pq.push(pqvec.back());
						pqvec.pop_back();
					}
				}
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
	while (curr != nullptr) {
		path->insert(path->begin(), curr);
		curr = parents[curr];
	}

	return path;
}

/* generates a configuartion space given a list of obstacles (circles-only) and agents (circle-only; 1-only) */
Cspace_2D::Cspace_2D(std::vector<Circle> & obs, Circle & agent) {
	init(obs.data(), obs.size(), agent);
}

Cspace_2D::Cspace_2D(Circle * obs, int n, Circle & agent) {
	init(obs, n, agent);
}

void Cspace_2D::init(Circle * obs, int n, Circle & agent) {
	this->obs_circle = new std::vector<Circle>();
	for (int i = 0; i < n; i++) {
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
		Point o = (*this->obs_circle)[i].o;
		float r = (*this->obs_circle)[i].r;
		Point diff = subP(p, o);
		if (dotP(diff, diff) <= r * r)
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

/* detects if a line segment between Point a and Point b collides with the C-space 
   method : projects the line (AC) onto (AB), determines if the projection is close 
   enough to the segment and if the rejection is within the circle
   I did this because it didn't involve any square roots. And I thought it was cool.
   Oh, and I was tired/delusional.
   Not even sure if it is more efficient than the classic ray-sphere, lol.
   It seems like there are more dot products, but hey! No square roots!

   If you want to play with it I made this:
   https://www.desmos.com/calculator/xnitdx0vw7
 */
bool Cspace_2D::lineOfSight(Point a, Point b) {
	printP(a, "a ");
	printP(b, "b ");

	Point Lab;
	Lab.x = b.x - a.x;
	Lab.y = b.y - a.y;
	printP(Lab, "Lab ");

	float len2 = dotP(Lab, Lab);
	std::cout << "len2 " << len2 << std::endl;

	for (int i = 0; i < this->obs_circle->size(); i++) {
		Circle c;
		c = this->obs_circle->at(i);
		printP(c.o, "co ");
		std::cout << "cr " << c.r << std::endl;

		Point Lao;
		Lao.x = c.o.x - a.x;
		Lao.y = c.o.y - a.y;
		printP(Lao, "Lao ");

		float r2 = c.r * c.r;
		std::cout << "r2 " << r2 << std::endl;
		std::cout << "Lao2 " << dotP(Lao, Lao) << std::endl;
		if (dotP(Lao, Lao) <= r2) {//point a inside circle
			std::cout << "HIT 1" << std::endl;
			return false; // HIT
		}

		Point Lbo;
		Lbo.x = c.o.x - b.x;
		Lbo.y = c.o.y - b.y;
		printP(Lbo, "Lbo ");
		std::cout << "Lbo2 " << dotP(Lbo, Lbo) << std::endl;
		if (dotP(Lbo, Lbo) <= r2) { //point b inside circle
			std::cout << "HIT 2" << std::endl;
			return false; // HIT
		}

		float ang = dotP(Lab, Lao);
		std::cout << "ang " << ang << std::endl;
		Point proj = scaleP(a, ang / len2);
		printP(proj, "proj ");
		Point rej = subP(b, proj);
		printP(rej, "rej ");
		float plen2 = dotP(proj, proj);
		std::cout << "plen2 " <<  plen2 << std::endl;

		std::cout << "rej2 " << dotP(rej, rej) << std::endl;
		std::cout << "1 " << (dotP(rej, rej) <= r2) << std::endl;
		std::cout << "2 " << (0 <= ang) << std::endl;
		std::cout << "3 " << (plen2 <= len2) << std::endl;
		if (dotP(rej, rej) <= r2  //close enough tangentially
				&& 0 <= ang       //point a before circle center
				&& plen2 <= len2) { //point b after circle center

			std::cout << "HIT 3" << std::endl;
			return false; // HIT
		}
	}

	std::cout << "MISS" << std::endl;
	return true; // MISS
}
