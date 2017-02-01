vector4 a = new vector4(255, 195, 0, 255);
vector4 b = new vector4(199, 0, 57, 190);

void simuAttrs(float dt, int i) {
  /* causing surprising amount of K-loss. Roughly 8K */
  /* the issues is not a color instantiation... */
  /* the issue is the color primitive. switching to "vector4" (mine), 
     mitigates the loss to roughly 3K. Still significant for what lerp is */
  clr[i] = lerp(a, b, life[i]/maxlife[i]);//(pos[i].y)/(0.25*height));//
}
  
/* currently causing little to no K-loss */
void computePhysics(float dt, int i) { 
  //float tx = padX;
  //float tz = padZ;
  //float dx = (pos[i].x - tx);
  //float dz = (pos[i].z - tz);
  //vel[i].x = vel[i].x - dx/3 * dt;
  //(width*width/4 - distToPull2)/2000 * dt;
  //vel[i].z = vel[i].z - dz/3 * dt;
  
  //eulerian
  vel[i].x = vel[i].x + acc[i].x * dt;
  vel[i].y = vel[i].y + acc[i].y * dt;
  vel[i].z = vel[i].z + acc[i].z * dt;
  pos[i].x = pos[i].x + vel[i].x * dt;
  pos[i].y = pos[i].y + vel[i].y * dt;
  pos[i].z = pos[i].z + vel[i].z * dt;
  
  checkForCollisions(i);
}

void checkForCollisions(int i) {
  //my sphere
  
  vector3 toS = new vector3(pos[i].x - padX, pos[i].y - padY, pos[i].z - padZ);
  float mag2 = toS.dot(toS);
  if (mag2 < padR*padR) {
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
  if (pos[i].y + radius >= floor) {
    pos[i].y = floor - radius - .95*(pos[i].y -floor);
    vel[i].y *= -.95;
  } 
}