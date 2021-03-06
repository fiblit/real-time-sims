void simuAttrs(float dt, int i) {
  /* causing surprising amount of K-loss. Roughly 8K */
  /* the issues is not a color instantiation... */
  /* the issue is the color primitive. switching to "vector4" (mine), 
     mitigates the loss to roughly 3K. Still significant for what lerp is */
  clr[i] = lerp(clrRange[i].l, clrRange[i].h, life[i]/maxlife[i]);//(pos[i].y)/(0.25*height));//
}
  
/* currently causing little to no K-loss */
void computePhysics(float dt, int i) {
  calculateForces(i);
  
  //eulerian
  vel[i].x = vel[i].x + acc[i].x * dt;
  vel[i].y = vel[i].y + acc[i].y * dt;
  vel[i].z = vel[i].z + acc[i].z * dt;
  pos[i].x = pos[i].x + vel[i].x * dt;
  pos[i].y = pos[i].y + vel[i].y * dt;
  pos[i].z = pos[i].z + vel[i].z * dt;
  
  checkForCollisions(i);
}

void calculateForces(int i) {
  //update acc[i]  
  /*
  if (eRef[i].isPlume)//do not use index for eRef if you do it.
    float dx = (pos[i].x - eref[i].x);
    float dz = (pos[i].z - eref[i].z);
    acc[i].x = - dx/3;
    float rad = eRef[i].radius[1] - eRef[i].radius[0];
    acc[i].y = (rad*rad - dx*dx + dz * dz)/2000;
    acc[i].z = - dz/3;
  }
  */
}

void checkForCollisions(int i) {
  //paddle
  vector3 toS = new vector3(pos[i].x - padX, pos[i].y - padY, pos[i].z - padZ);
  float mag2 = toS.dot(toS);
  if (mag2 < (padR+radius)*(padR+radius)) {
    float mag  = sqrt(mag2);
    vector3 toSNorm = new vector3(toS.x / mag, toS.y / mag, toS.z / mag );
    float alpha = 0.95;// (0,1] 
    float co = (1+alpha)*vel[i].dot(toSNorm);
    vel[i] = new vector3(
      (vel[i].x - co * toSNorm.x),
      (vel[i].y - co * toSNorm.y),
      (vel[i].z - co * toSNorm.z));
    pos[i] = new vector3(
      (padX  + toSNorm.x * (padR+radius)),
      (padY  + toSNorm.y * (padR+radius)),
      (padZ + toSNorm.z * (padR+radius)));
  }
  
  //floor
  if (pos[i].y + radius*1.01 >= floor) {
    pos[i].y = floor - radius - .95*(pos[i].y -floor);
    vel[i].y *= -.95;
  } 
}