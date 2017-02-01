void renderLighting() {
  directionalLight(255, 255, 255, -0.5, 1, 0);
}

void renderObjects() {
  beginShape(QUAD);
  fill(0);
  vertex(1000,floor,1000);
  vertex(1000,floor,-1000);
  vertex(-1000,floor,-1000);
  vertex(-1000,floor,1000);
  endShape();
  
  pushMatrix();  
  translate(padX,padY,padZ);
  fill(0,0,0,255);
  sphere(padR);
  popMatrix();
}

float cY, sY, cP, sP;
float[][] vpp = new float[3][3];
float[][] v = new float[3][3];
/* TODO: test if manual unrolling has difference */
void render(int i) {
  fill(clr[i].x, clr[i].y, clr[i].z);//, clr[i].w);
  
  //translate (I don't trust the interpreter to unroll for me
  v[0][0] = vpp[0][0] + pos[i].x; v[1][0] = vpp[1][0] + pos[i].x; v[2][0] = vpp[2][0] + pos[i].x;
  v[0][1] = vpp[0][1] + pos[i].y; v[1][1] = vpp[1][1] + pos[i].y; v[2][1] = vpp[2][1] + pos[i].y;
  v[0][2] = vpp[0][2] + pos[i].z; v[1][2] = vpp[1][2] + pos[i].z; v[2][2] = vpp[2][2] + pos[i].z;
  
  //obviously the biggest FPS culprit
  //(I don't trust the interpreter to unroll for me
  vertex(v[0][0],v[0][1],v[0][2]);
  vertex(v[1][0],v[1][1],v[1][2]);
  vertex(v[2][0],v[2][1],v[2][2]);
}

void faceCam() {
  float yawi = -yaw-90;//not really sure why this is necessary
  cY = cos(yawi*PI/180);  // pull these constant calculations out of per particle render
  sY = sin(yawi*PI/180);
  cP = cos(pitch*PI/180);
  sP = sin(pitch*PI/180);

  //rot round X then round Y
  vpp[0][0] =  cY * vTri[0][0] + sP*sY*vTri[0][1] + cP*sY * vTri[0][2];
  vpp[0][1] =  0  * vTri[0][0] + cP *  vTri[0][1] - sP    * vTri[0][2];
  vpp[0][2] = -sY * vTri[0][0] + sP*cY*vTri[0][1] + cY*cP * vTri[0][2];
  
  vpp[1][0] =  cY * vTri[1][0] + sP*sY*vTri[1][1] + cP*sY * vTri[1][2];
  vpp[1][1] =  0  * vTri[1][0] + cP *  vTri[1][1] - sP    * vTri[1][2];
  vpp[1][2] = -sY * vTri[1][0] + sP*cY*vTri[1][1] + cY*cP * vTri[1][2];
  
  vpp[2][0] =  cY * vTri[2][0] + sP*sY*vTri[2][1] + cP*sY * vTri[2][2];
  vpp[2][1] =  0  * vTri[2][0] + cP *  vTri[2][1] - sP    * vTri[2][2];
  vpp[2][2] = -sY * vTri[2][0] + sP*cY*vTri[2][1] + cY*cP * vTri[2][2];
}

void noFace() {
  vpp[0][0] = vTri[0][0]; vpp[0][1] = vTri[0][1]; vpp[0][2] = vTri[0][2];
  vpp[1][0] = vTri[1][0]; vpp[1][1] = vTri[1][1]; vpp[1][2] = vTri[1][2];
  vpp[2][0] = vTri[2][0]; vpp[2][1] = vTri[2][1]; vpp[2][2] = vTri[2][2];
}