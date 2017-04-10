#ifndef GLOBAL_MOTION_PLANNER_H_GUARD
#define GLOBAL_MOTION_PLANNER_H_GUARD

#include "PRM.hpp"
#include <vector>

namespace GMP {
    VecPoint * findPathUCS(Graph<Point> * roadmap);
    VecPoint * findPathAstar(float e, Graph<Point> * roadmap);
};

#endif//GLOBAL_MOTION_PLANNER_H_GUARD