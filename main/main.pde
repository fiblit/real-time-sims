/** particle attributes **/
vector3[] pos, vel, acc; 
float[] life, maxlife;
float[] mass;
vector4[] clr;
float[][] vTri = new float[3][3];

int pcount; 
float floor;

void setup() {
  size(1200, 1000, P3D);
  floor = 2*height;
  regen();
  noStroke();
  frameNow = millis();
  smooth(2);
  //noSmooth();

  //float hry = radius ;//* pos[i].y / (0.75 * height);
  //triangle(0, -hry*hry*0.4330127, 
  //radius/2, hry*hry*0.4330127,
  //-radius/2, hry*hry*0.4330127);
  //init
  vTri[0][0] = 0        ; vTri[0][1] = -radius/2; vTri[0][2] = 0;
  vTri[1][0] = -radius/2; vTri[1][1] = radius/2 ; vTri[1][2] = 0;
  vTri[2][0] =  radius/2; vTri[2][1] = radius/2 ; vTri[2][2] = 0;
  
  if (pFaceCam) {
    faceCam();
  }
  else {
    noFace();
  }

  forward = new vector3(
    cos(yaw*PI/180)*cos(pitch*PI/180),
    sin(pitch*PI/180),
    sin(yaw*PI/180)*cos(pitch*PI/180));
  right = new vector3(
    cos((yaw+90)*PI/180)*cos(pitch*PI/180),
    sin(pitch*PI/180),
    sin((yaw+90)*PI/180)*cos(pitch*PI/180)); 
}  

void draw() {
  background(255, 255, 255, 0);
  tick();
  
  if (keyPressed)
    userInput(sdt);
  textSize(32); 
  fill(0, 0, 0);
  text("FPS:"+int(frameRate)+"; \nparticles: " +pcount, -1000, 1000);
  camera(
    eye.x, eye.y, eye.z, 
    eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
    0, 1, 0);
  p_gen(sdt);

  /* the ordering of objects and particles is very specific */
  renderLighting();
  renderObjects();
  noLights();
  //gives a reasonable performance boost over old method. (Roughly 5 to 10K, never got an exact count)
  beginShape(TRIANGLES);
  for (int i = 0; i < pcount; i++) {
    computePhysics(sdt, i);
    simuAttrs(sdt, i);
    render(i);
    if (p_kill(sdt, i))
      //since we swapped from the end. We need to process what was at the end.
      i--;
  }
  endShape();
}