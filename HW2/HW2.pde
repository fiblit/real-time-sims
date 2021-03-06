import java.text.DecimalFormat;
Cloth myCloth;
void setup() {
  size(displayWidth, displayHeight, P3D);
  //fullScreen(P3D);
  
  noStroke();
  smooth(2);
  gen();

  //cannot be moved to settings. Height is init before setup.
  eye = new Vec3(0, 0, 1300);
  frameNow = millis();
  
  vpp = new float[pMesh.length][3];
  v = new float[pMesh.length][3];
  
  cameraCalc();
    
  for (Emitter e: initEmitters) {
    emitters.add(e);
  }
  
  int res = 30;
  myCloth = new Cloth(
    res, res, 
    new Vec3(0,-500,0), 
    new Vec3(1000,0,0), 
    new Vec3(0,0,1000), 
    1000000, 1000000, 1000/res);
    
  textureMode(NORMAL);
  img = loadImage("BabyPanda.JPG");
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
  //noLights();
  
  fill(255,0,0);
  //println(sdt);
  float targetDT = 0.0001;//.1ms
  float currDT = sdt;
  int simpf = 0;
  if (currDT > 1/60.0) currDT = 1/60.0;
  while(currDT > 0) {
    float beforeP = millis();
    myCloth.computePhysics(targetDT);
    myCloth.checkCollisions();
    currDT -= max((millis()-beforeP)/1000, targetDT);
    simpf++;
  }  
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
  DecimalFormat df = new DecimalFormat();
  df.setMaximumFractionDigits(5);
  text("--- BUILD: DEBUG ---\nFPS: "+df.format(frameRate)+"\ntarget-dt:"+df.format(targetDT*1000)+"ms Cloth Physics Sim/frame:"+simpf+"\nparticles: " + pcount +
  " \n\nmode:\n    " + getModeName() +
  "\nB to build that scene; X to spawn that emitter \nTG to change mode" +
  "\n\nWASD to strafe \nZC to move up&down \nQERF to pan camera \nIJKLUO to move paddle\nGo away ;_;", -1000, 550);
}