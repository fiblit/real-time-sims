void renderLighting() {
  directionalLight(255, 255, 255, -0.5, 1, -0.5);
}

void renderObjects() {
  
  pushMatrix();  
  translate(padX,padY,padZ);
  fill(0,0,0,255);
  sphere(padR);
  popMatrix();
  
  beginShape(QUAD);
  fill(100,100,100);
  vertex(2000,floor,2000);
  vertex(2000,floor,-2000);
  vertex(-2000,floor,-2000);
  vertex(-2000,floor,2000);
  
  fill(120,120,120,120);//???
  vertex(-1200,550,-10);
  vertex(-1200,1200,-10);
  vertex(-200,1200,-10);
  vertex(-200,550,-10);
  endShape();
}

float cY, sY, cP, sP;
float[][] vpp;// = new float[3][3];
float[][] v;// = new float[3][3];
/* TODO: test if manual unrolling has difference */
void p_render(int i) {
  fill(ps[i].clr.x, ps[i].clr.y, ps[i].clr.z, ps[i].clr.w);
  
  if (ps[i].isBall) {
    endShape();
    pushMatrix();
    translate(ps[i].pos.x, ps[i].pos.y, ps[i].pos.z);
    sphere(radius);
    popMatrix();
    beginShape(TRIANGLES);
  }
  else {
    for (int p = 0; p < vpp.length; p++) {
  
        v[p][0] = vpp[p][0] + ps[i].pos.x;
        v[p][1] = vpp[p][1] + ps[i].pos.y;
        v[p][2] = vpp[p][2] + ps[i].pos.z;
      //obviously the biggest FPS culprit
      vertex(v[p][0],v[p][1],v[p][2]);
    }
  }
}

void faceCam() {
  float yawi = -yaw-90;//not really sure why this is necessary
  cY = cos(yawi*PI/180);  // pull these constant calculations out of per particle render
  sY = sin(yawi*PI/180);
  cP = cos(pitch*PI/180);
  sP = sin(pitch*PI/180);

  //rot round X then round Y
  for (int p = 0; p < vpp.length; p++) {
    vpp[p][0] =  cY * pMesh[p][0] + sP*sY*pMesh[p][1] + cP*sY * pMesh[p][2];
    vpp[p][1] =  0  * pMesh[p][0] + cP *  pMesh[p][1] - sP    * pMesh[p][2];
    vpp[p][2] = -sY * pMesh[p][0] + sP*cY*pMesh[p][1] + cY*cP * pMesh[p][2];
  }
}

void noFace() {
  for (int p = 0; p < vpp.length; p++) {
    for (int c = 0; c < 3; c++) {
      vpp[p][c] = pMesh[p][c];    
    }
  }
}