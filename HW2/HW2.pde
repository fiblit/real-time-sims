Cloth myCloth;
void setup() {
  size(1200, 1000, P3D);
  noStroke();
  smooth(2);
  gen();
  
  //smoke = loadImage("smoke.png");
  //smoke.mask(loadImage("smokemask.png"));
  //image(smoke,0,0);
  //textureMode(NORMAL);
  
  //cannot be moved to settings. Height is init before setup.
  eye = new Vec3(-1300, 1100, 1300);
  frameNow = millis();
  
  vpp = new float[pMesh.length][3];
  v = new float[pMesh.length][3];
  
  cameraCalc();
    
  for (Emitter e: initEmitters) {
    emitters.add(e);
  }
  
  myCloth = new Cloth(
    10, 10, 
    new Vec3(1000,0,0), 
    new Vec3(50,0,0), 
    new Vec3(0,50,0), 
    5, 1, 5);
}  

String getModeName() {
  String s = new Integer(emitterMode).toString() + ": ";
  switch(emitterMode) {
    case 0: s += "Bouncy Ball (might need to wait for one to spawn)"; break;
    case 1: s += "Spout of WATER!"; break;
    case 2: s += "Heh. Fire."; break;
    case 3: s += "Magic Missle!"; break;
    case 4: s += "Fireball. 100k@30FPS bench attempt"; break;
    case 5: s += "Fireball. 90K@30+FPS bench"; break;
    case 6: s += "Snow!"; break;
    case 7: s += "Blank Canvas"; break;
  }
  return s;
}

void draw() {
  tick();
  
  if (keyPressed)
    userInput(sdt);
    
  camera(
    eye.x, eye.y, eye.z, 
    eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
    0, 1, 0);

  p_gen(sdt);

  /* the ordering of objects and particles is very specific */
  background(255, 255, 255, 0);
  renderLighting();
  //sphereDetail(30);
  renderObjects();
  noLights();
  
  fill(255,0,0);
  myCloth.computePhysics(sdt);
  myCloth.render();
  
  beginShape(TRIANGLES);
  for (int i = 0; i < pcount; i++) {
    p_computePhysics(sdt, i);
    p_simuAttrs(sdt, i);
    p_render(i);
    if (p_kill(sdt, i))
      //since we swapped from the end. We need to process what was at the end.
      i--;
  }
  endShape();  
  
  e_kill(sdt);
  
  textSize(32); 
  fill(255);
  text("--- BUILD: DEBUG ---\nFPS: "+frameRate+"\nparticles: " + pcount +
  " \n\nmode:\n    " + getModeName() +
  "\nB to build that scene; X to spawn that emitter \nTG to change mode" +
  "\n\nWASD to strafe \nZC to move up&down \nQERF to pan camera \nIJKLUO to move paddle\nGo away ;_;", -1000, 600);
}