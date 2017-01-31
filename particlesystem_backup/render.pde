
void render(int i) {
  //fill marginally hurts FPS. Maybe a few K less @ 30FPS
  fill(clr[i].x, clr[i].y, clr[i].z, clr[i].w);

  //float hry = radius ;//* pos[i].y / (0.75 * height);
  //triangle(0, -hry*hry*0.4330127, 
  //radius/2, hry*hry*0.4330127,
  //-radius/2, hry*hry*0.4330127);
  float[][] v = new float[3][3];
  //init
  v[0][0] = 0        ; v[0][1] = -radius/2; v[0][2] = 0;
  v[1][0] = -radius/2; v[1][1] = radius/2 ; v[1][2] = 0;
  v[2][0] =  radius/2; v[2][1] = radius/2 ; v[2][2] = 0;
  
  //rotate to face (approx.) assumes orthogonality, which for this FOV is fine...
  float yawi = -yaw-90;//not really sure why this is necessary
  float[][] vt = new float[3][3]; //it was so buggy without this
  
  //[ cY , sPsY, cPsY]
  //[ 0  , cP  , -sP ] (rot by P around X)
  //[-sY , sPcY, cPcY]  (then rot by Y around new Y) 
  float cY = cos(yawi*PI/180);
  float sY = sin(yawi*PI/180);
  float cP = cos(pitch*PI/180);
  float sP = sin(pitch*PI/180);
  //(I don't trust the compiler to unroll for me
  vt[0][0] =  cY * v[0][0] + sP*sY*v[0][1] + cP*sY * v[0][2];
  vt[0][1] =  0  * v[0][0] + cP *  v[0][1] - sP    * v[0][2];
  vt[0][2] = -sY * v[0][0] + sP*cY*v[0][1] + cY*cP * v[0][2];
  
  vt[1][0] =  cY * v[1][0] + sP*sY*v[1][1] + cP*sY * v[1][2];
  vt[1][1] =  0  * v[1][0] + cP *  v[1][1] - sP    * v[1][2];
  vt[1][2] = -sY * v[1][0] + sP*cY*v[1][1] + cY*cP * v[1][2];
  
  vt[2][0] =  cY * v[2][0] + sP*sY*v[2][1] + cP*sY * v[2][2];
  vt[2][1] =  0  * v[2][0] + cP *  v[2][1] - sP    * v[2][2];
  vt[2][2] = -sY * v[2][0] + sP*cY*v[2][1] + cY*cP * v[2][2];
  
  //translate (I don't trust the compiler to unroll)
  v[0][0] = vt[0][0] + pos[i].x; v[1][0] = vt[1][0] + pos[i].x; v[2][0] = vt[2][0] + pos[i].x;
  v[0][1] = vt[0][1] + pos[i].y; v[1][1] = vt[1][1] + pos[i].y; v[2][1] = vt[2][1] + pos[i].y;
  v[0][2] = vt[0][2] + pos[i].z; v[1][2] = vt[1][2] + pos[i].z; v[2][2] = vt[2][2] + pos[i].z;
  
  //obviously the biggest FPS culprit
  //I don't trust it to unroll for me
  vertex(v[0][0],v[0][1], v[0][2]);
  vertex(v[1][0],v[1][1], v[1][2]);
  vertex(v[2][0],v[2][1], v[2][2]);
}