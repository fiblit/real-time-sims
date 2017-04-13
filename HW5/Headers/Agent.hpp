#ifndef AGENT_H_GUARD
#define AGENT_H_GUARD

#include <vector>

#include "PRM.hpp"

namespace agent {
    enum class volume_type { CIRC, RECT };
};

class Agent {
public:
    glm::vec2 start, goal;
    Cspace_2D * cspace; //wish I could split this up into static/dynamic
    PRM * prm; //this seems wasteful...?

    std::vector<Node<glm::vec2> *> * plan;
    int completed_nodes;
    //"Model" -- nope -- too much work RN 
    agent::volume_type vt;
    BoundingVolume * bv;
    /*
    Agent(agent::volume_type vt){///*, glm::vec2 start, glm::vec2 goal//) {
        this->vt = vt;
        if (vt == agent::volume_type::CIRC)
            this->bv = new Circ();
        else
            this->bv = new Rect();
    }
    */
    Agent(BoundingVolume * bv, glm::vec2 goal) {
        this->bv = bv;
        this->start = bv->o;
        this->goal = goal;
    }
};


#endif//AGENT_H_GUARD