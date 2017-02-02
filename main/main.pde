/** particle attributes **/
vector3[] pos, vel, acc; 
float[] life, maxlife;
float[] mass;
vector4[] clr;
float floor;
int pcount;

void setup() {
  size(800, 600, P3D);
  noStroke();
  smooth(2);
  gen();
  
  //cannot be moved to settings. Height is init before setup.
  floor = 2*height;
  frameNow = millis();
  
  vpp = new float[pMesh.length][3];
  v = new float[pMesh.length][3];
  
  cameraCalc();
    
  for (emitter e: initEmitters) {
    emitters.add(e);
  }
}  

void draw() {
  background(255, 255, 255, 0);
  tick();
  
  if (keyPressed)
    userInput(sdt);
  textSize(32); 
  fill(0, 0, 0);
  text("FPS:"+frameRate+" \nparticles: " +pcount +
  " \nmode: " + emitterMode, -1000, 1000);
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