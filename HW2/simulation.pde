void simuAttrs(float dt, int i) {
  /* causing surprising amount of K-loss. Roughly 8K */
  /* the issues is not a color instantiation... */
  /* the issue is the color primitive. switching to "vector4" (mine), 
     mitigates the loss to roughly 3K. Still significant for what lerp is */
  ps[i].clr = lerp(ps[i].clrRange.l, ps[i].clrRange.h, ps[i].life/ps[i].maxlife);//(ps[i].pos.y)/(0.25*height));//
}
  
/* currently causing little to no K-loss */
void computePhysics(float dt, int i) {
  calculateForces(i);
  
  //eulerian
  ps[i].vel.x = ps[i].vel.x + ps[i].acc.x * dt;
  ps[i].vel.y = ps[i].vel.y + ps[i].acc.y * dt;
  ps[i].vel.z = ps[i].vel.z + ps[i].acc.z * dt;
  ps[i].pos.x = ps[i].pos.x + ps[i].vel.x * dt;
  ps[i].pos.y = ps[i].pos.y + ps[i].vel.y * dt;
  ps[i].pos.z = ps[i].pos.z + ps[i].vel.z * dt;
  
  checkForCollisions(i);
}

void calculateForces(int i) {
  //update acc[i]  
  /*
  if (eRef[i].isPlume)//do not use index for eRef if you do it.
    float dx = (ps[i].pos.x - eref[i].x);
    float dz = (ps[i].pos.z - eref[i].z);
    acc[i].x = - dx/3;
    float rad = eRef[i].radius[1] - eRef[i].radius[0];
    acc[i].y = (rad*rad - dx*dx + dz * dz)/2000;
    acc[i].z = - dz/3;
  }
  */
}

void checkForCollisions(int i) {
  //paddle
  vector3 toS = new vector3(ps[i].pos.x - padX, ps[i].pos.y - padY, ps[i].pos.z - padZ);
  float mag2 = toS.dot(toS);
  if (mag2 < (padR+radius)*(padR+radius)) {
    float mag  = sqrt(mag2);
    vector3 toSNorm = new vector3(toS.x / mag, toS.y / mag, toS.z / mag );
    float alpha = 0.95;// (0,1] 
    float co = (1+alpha)*ps[i].vel.dot(toSNorm);
    ps[i].vel = new vector3(
      (ps[i].vel.x - co * toSNorm.x),
      (ps[i].vel.y - co * toSNorm.y),
      (ps[i].vel.z - co * toSNorm.z));
    ps[i].pos = new vector3(
      (padX  + toSNorm.x * (padR+radius)),
      (padY  + toSNorm.y * (padR+radius)),
      (padZ + toSNorm.z * (padR+radius)));
  }
  
  //floor
  if (ps[i].pos.y + radius*1.01 >= floor) {
    ps[i].pos.y = floor - radius - .95*(ps[i].pos.y -floor);
    ps[i].vel.y *= -.95;
  } 
}