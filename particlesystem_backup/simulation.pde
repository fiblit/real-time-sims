vector4 a = new vector4(255, 195, 0, 255);
vector4 b = new vector4(199, 0, 57, 190);
void simuAttrs(float dt, int i) {
  /* causing surprising amount of K-loss. Roughly 8K */
  /* the issues is not a color instantiation... */
  /* the issue is the color primitive. switching to "vector4" (mine), 
     mitigates the loss to roughly 3K. Still significant for what lerp is */
  clr[i] = lerp(a, b, (pos[i].y)/(0.25*height));
}

float acceleration = 98;  
/* currently causing little to no K-loss */
void computePhysics(float dt, int i) { 
  float tx = 500;
  float tz = -500;
  float dx = (pos[i].x - tx);
  float dz = (pos[i].z - tz);
  float distToPull2 = dx*dx + dz *dz;
  vel[i].y = vel[i].y + acceleration * dt;//(width*width/4 - distToPull2)/2000 * dt;
  //vel[i].x = vel[i].x - dx/3 * dt;
  //vel[i].z = vel[i].z - dz/3 * dt;
  pos[i].y = pos[i].y + vel[i].y * dt;
  pos[i].x = pos[i].x + vel[i].x * dt;
  pos[i].z = pos[i].z + vel[i].z * dt;
  
  checkForCollisions(i);
}

void checkForCollisions(int i) {
  vector3 toS = new vector3(pos[i].x - 450, pos[i].y - 400, pos[i].z + 450);
  float mag = sqrt(toS.dot(toS));
  if (mag < 500) {
    vector3 toSNorm = new vector3(toS.x / mag, toS.y / mag, toS.z / mag );
    float co = 2*vel[i].dot(toSNorm);
    vel[i] = new vector3(
      (vel[i].x - co * toSNorm.x) * .95,
      (vel[i].y - co * toSNorm.y) * .95,
      (vel[i].z - co * toSNorm.z) * .95);
  }
  
  if (pos[i].y + radius >= floor) {
    pos[i].y = floor - radius - .95*(pos[i].y -floor);
    vel[i].y *= -.95;
  }
  
}