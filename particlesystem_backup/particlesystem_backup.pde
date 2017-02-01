/** particle attributes **/
vector3[] pos, vel; 
float[] life;
vector4[] clr;

int rgpcount, pcount; 
float floor;

void setup() {
  size(800, 600, P3D);
  floor = height;
  rgpcount = startcount;
  regen();
  noStroke();
  frameNow = millis();
  smooth(2);
  //noSmooth();
}  

void draw() {
  background(255, 255, 255, 0);
  tick();
  
  if (keyPressed)
    userInput();
  
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